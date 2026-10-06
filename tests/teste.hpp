#pragma once

// Mini framework de testes, sem dependências externas.
//
// Uso:
//   TESTE(nome_do_teste)
//   {
//     VERIFICA(expressao);
//     VERIFICA_IGUAL(obtido, esperado);
//   }
//
// Cada TESTE se registra sozinho; tests/main.cpp executa todos.

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace teste {

struct Caso
{
  const char* nome;
  void (*funcao)();
};

// static local evita problema de ordem de inicialização entre arquivos
inline std::vector<Caso>& registro()
{
  static std::vector<Caso> casos;
  return casos;
}

// falhas do teste em execução
inline int& falhas_atuais()
{
  static int falhas = 0;
  return falhas;
}

struct Registrador
{
  Registrador(const char* nome, void (*funcao)())
  {
    registro().push_back({ nome, funcao });
  }
};

inline void falha(const char* arquivo, int linha, const std::string& mensagem)
{
  ++falhas_atuais();
  std::cerr << "    " << arquivo << ":" << linha << ": " << mensagem << "\n";
}

inline bool quase_igual(double a, double b, double tolerancia = 1e-9)
{
  return std::fabs(a - b) <= tolerancia;
}

// retorna 0 se todos passaram, 1 caso contrário
inline int executa_todos()
{
  int reprovados = 0;
  for (const Caso& caso : registro()) {
    falhas_atuais() = 0;
    caso.funcao();
    if (falhas_atuais() == 0) {
      std::cout << "[ OK ] " << caso.nome << "\n";
    } else {
      std::cout << "[FALHA] " << caso.nome << "\n";
      ++reprovados;
    }
  }

  std::cout << "\n"
            << registro().size() - reprovados << "/" << registro().size()
            << " testes passaram\n";
  return reprovados == 0 ? 0 : 1;
}

} // namespace teste

#define TESTE(nome)                                                            \
  static void teste_##nome();                                                  \
  static teste::Registrador registrador_##nome(#nome, teste_##nome);           \
  static void teste_##nome()

#define VERIFICA(expressao)                                                    \
  do {                                                                         \
    if (!(expressao))                                                          \
      teste::falha(__FILE__, __LINE__, "falhou: " #expressao);                 \
  } while (0)

#define VERIFICA_IGUAL(obtido, esperado)                                       \
  do {                                                                         \
    if (!((obtido) == (esperado)))                                             \
      teste::falha(__FILE__, __LINE__, #obtido " != " #esperado);              \
  } while (0)

#define VERIFICA_PROXIMO(obtido, esperado)                                     \
  do {                                                                         \
    if (!teste::quase_igual((obtido), (esperado)))                             \
      teste::falha(__FILE__, __LINE__, #obtido " !~ " #esperado);              \
  } while (0)

#define VERIFICA_LANCA(expressao, tipo_excecao)                                \
  do {                                                                         \
    bool lancou = false;                                                       \
    try {                                                                      \
      (void)(expressao);                                                       \
    } catch (const tipo_excecao&) {                                            \
      lancou = true;                                                           \
    } catch (...) {                                                            \
    }                                                                          \
    if (!lancou)                                                               \
      teste::falha(                                                            \
        __FILE__, __LINE__, #expressao " nao lancou " #tipo_excecao);          \
  } while (0)
