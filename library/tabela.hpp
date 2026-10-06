#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// Tabela de texto com colunas alinhadas.
// A largura de cada coluna é a da maior célula (ou do título).
//
//   +---------+-------+
//   | Vertice | Grau  |
//   +---------+-------+
//   | A       |     2 |
//   +---------+-------+
class Tabela
{
public:
  enum class Alinhamento
  {
    esquerda,
    direita
  };

  struct Coluna
  {
    std::string titulo;
    Alinhamento alinhamento = Alinhamento::esquerda;
  };

private:
  std::vector<Coluna> m_colunas;
  // uma linha vazia representa um separador horizontal
  std::vector<std::vector<std::string>> m_linhas;

public:
  explicit Tabela(std::vector<Coluna> colunas);

  // lança std::invalid_argument se o número de células for diferente
  // do número de colunas
  void adiciona_linha(std::vector<std::string> celulas);

  void adiciona_separador();

  void imprime(std::ostream& saida = std::cout) const;

private:
  std::vector<std::size_t> larguras() const;

  void imprime_separador(std::ostream& saida,
                         const std::vector<std::size_t>& larguras) const;

  void imprime_celulas(std::ostream& saida,
                       const std::vector<std::string>& celulas,
                       const std::vector<std::size_t>& larguras) const;
};
