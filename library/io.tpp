#pragma once

#include "entrada.hpp"
#include "grafo.hpp"
#include "tabela.hpp"
#include "texto.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// converte um valor em texto; ponto flutuante sai com `casas` casas decimais
template<typename T>
std::string formata(const T& valor, int casas = 2)
{
  std::ostringstream saida;
  if constexpr (std::is_floating_point_v<T>)
    saida << std::fixed << std::setprecision(casas);
  saida << valor;
  return saida.str();
}

inline std::string junta(const std::vector<std::string>& partes,
                         const std::string& separador)
{
  std::string resultado;
  for (std::size_t i = 0; i < partes.size(); ++i) {
    if (i > 0)
      resultado += separador;
    resultado += partes[i];
  }
  return resultado;
}

// imprime os descendentes de `v` no estilo do comando `tree`;
// `prefixo` carrega as barras verticais dos níveis acima
template<typename Rotulo>
void imprime_filhos(vertice_id v,
                    const std::vector<std::vector<vertice_id>>& filhos,
                    const Rotulo& rotulo,
                    const std::string& prefixo)
{
  for (std::size_t i = 0; i < filhos[v].size(); ++i) {
    bool ultimo = i + 1 == filhos[v].size();
    vertice_id filho = filhos[v][i];

    std::cout << prefixo << (ultimo ? "└── " : "├── ") << rotulo(filho) << '\n';
    imprime_filhos(filho, filhos, rotulo, prefixo + (ultimo ? "    " : "│   "));
  }
}

// filhos[v] = filhos de v; rotulo(v) = texto exibido para o nó v
template<typename Rotulo>
void imprime_hierarquia(vertice_id raiz,
                        const std::vector<std::vector<vertice_id>>& filhos,
                        const Rotulo& rotulo)
{
  std::cout << rotulo(raiz) << '\n';
  imprime_filhos(raiz, filhos, rotulo, "");
}

template<typename V, typename P, bool D>
void carrega_grafo(const std::string& caminho, Grafo<V, P, D>& grafo)
{
  std::ifstream arquivo(caminho);

  if (!arquivo.is_open()) {
    throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminho);
  }

  std::string linha;

  while (std::getline(arquivo, linha)) {
    if (linha.empty())
      continue;

    std::stringstream ss(linha);

    std::string origem;
    std::string destino;
    std::string peso_texto;

    if (!std::getline(ss, origem, ',') || !std::getline(ss, destino, ',') ||
        !std::getline(ss, peso_texto)) {
      throw std::runtime_error("Linha invalida: " + linha);
    }

    std::stringstream conversor(peso_texto);

    P peso;

    if (!(conversor >> peso)) {
      throw std::runtime_error("Peso invalido na linha: " + linha);
    }

    // rotulos sem acento: alinham nas tabelas e casam com o que o usuario
    // digita
    grafo.adiciona_aresta(
      remove_acentos(origem), remove_acentos(destino), peso);
  }
}

template<typename V, typename P, bool D>
void imprime_lista_adjacencia(const Grafo<V, P, D>& grafo)
{
  if (grafo.get_numero_vertices() == 0) {
    std::cout << "Grafo vazio.\n";
    return;
  }

  Tabela tabela({ { "Vertice" },
                  { "Grau", Tabela::Alinhamento::direita },
                  { "Vizinhos (peso)" } });

  for (vertice_id u = 0; u < grafo.get_numero_vertices(); ++u) {
    std::vector<std::string> vizinhos;

    for (const auto& [v, peso] : grafo.lista_vizinhos(u))
      vizinhos.push_back(formata(grafo.get_rotulo(v)) + " (" + formata(peso) +
                         ")");

    tabela.adiciona_linha({ formata(grafo.get_rotulo(u)),
                            formata(grafo.get_grau_por_id(u)),
                            junta(vizinhos, ", ") });
  }

  tabela.imprime();
}

template<typename V, typename P, bool D>
void imprime_estatisticas_grau(const Grafo<V, P, D>& grafo)
{
  if (grafo.get_numero_vertices() == 0) {
    std::cout << "Grafo vazio.\n";
    return;
  }

  vertice_id maior_id = 0;
  vertice_id menor_id = 0;

  std::size_t maior_grau = grafo.get_grau_por_id(0);

  std::size_t menor_grau = maior_grau;

  std::size_t soma = 0;

  for (vertice_id v = 0; v < grafo.get_numero_vertices(); ++v) {

    std::size_t grau = grafo.get_grau_por_id(v);

    soma += grau;

    if (grau > maior_grau) {
      maior_grau = grau;
      maior_id = v;
    }

    if (grau < menor_grau) {
      menor_grau = grau;
      menor_id = v;
    }
  }

  double media = static_cast<double>(soma) / grafo.get_numero_vertices();

  Tabela tabela({ { "Estatistica" },
                  { "Vertice" },
                  { "Grau", Tabela::Alinhamento::direita } });

  tabela.adiciona_linha(
    { "Maior grau", formata(grafo.get_rotulo(maior_id)), formata(maior_grau) });
  tabela.adiciona_linha(
    { "Menor grau", formata(grafo.get_rotulo(menor_id)), formata(menor_grau) });
  tabela.adiciona_linha({ "Grau medio", "-", formata(media) });

  tabela.imprime();
}

template<typename V, typename P, bool D>
void imprime_componentes(const Grafo<V, P, D>& grafo)
{
  auto resultado = grafo.get_componentes_conexas();

  std::cout << "Numero de componentes: " << resultado.quantidade() << "\n\n";

  Tabela tabela({ { "#", Tabela::Alinhamento::direita },
                  { "Tamanho", Tabela::Alinhamento::direita },
                  { "Vertices" } });

  for (std::size_t i = 0; i < resultado.componentes.size(); ++i) {
    std::vector<std::string> rotulos;

    for (vertice_id v : resultado.componentes[i])
      rotulos.push_back(formata(grafo.get_rotulo(v)));

    tabela.adiciona_linha({ formata(i + 1),
                            formata(resultado.componentes[i].size()),
                            junta(rotulos, ", ") });
  }

  tabela.imprime();
}

template<typename V, typename P, bool D>
void imprime_floresta_mst(const Grafo<V, P, D>& grafo,
                          const FlorestaMST<P>& floresta)
{
  std::cout << "Numero de arvores: " << floresta.arvores.size() << '\n';

  Tabela resumo({ { "Arvore" },
                  { "Arestas", Tabela::Alinhamento::direita },
                  { "Custo", Tabela::Alinhamento::direita } });

  std::size_t total_arestas = 0;

  for (std::size_t i = 0; i < floresta.arvores.size(); ++i) {

    const auto& arvore = floresta.arvores[i];

    std::cout << "\nArvore " << i + 1 << '\n';

    if (arvore.arestas.empty()) {
      std::cout << "(componente sem arestas)\n";
    } else {
      // a MST não tem raiz: enraíza na origem da primeira aresta
      // (no Prim, o vértice onde a árvore começou)
      const std::size_t n = grafo.get_numero_vertices();
      std::vector<std::vector<std::pair<vertice_id, P>>> ligacoes(n);
      for (const auto& aresta : arvore.arestas) {
        ligacoes[aresta.get_origem()].push_back(
          { aresta.get_destino(), aresta.get_peso() });
        ligacoes[aresta.get_destino()].push_back(
          { aresta.get_origem(), aresta.get_peso() });
      }

      const vertice_id raiz = arvore.arestas.front().get_origem();
      std::vector<std::vector<vertice_id>> filhos(n);
      std::vector<std::optional<P>> peso_ate_pai(n);
      std::vector<bool> visitado(n, false);
      std::vector<vertice_id> pilha{ raiz };
      visitado[raiz] = true;

      while (!pilha.empty()) {
        vertice_id u = pilha.back();
        pilha.pop_back();
        for (const auto& [v, peso] : ligacoes[u]) {
          if (!visitado[v]) {
            visitado[v] = true;
            filhos[u].push_back(v);
            peso_ate_pai[v] = peso;
            pilha.push_back(v);
          }
        }
      }

      imprime_hierarquia(raiz, filhos, [&](vertice_id v) {
        std::string texto = formata(grafo.get_rotulo(v));
        if (peso_ate_pai[v])
          texto += " (" + formata(*peso_ate_pai[v]) + ")";
        return remove_acentos(texto);
      });
      std::cout << '\n';

      Tabela arestas({ { "Origem" },
                       { "Destino" },
                       { "Peso", Tabela::Alinhamento::direita } });

      for (const auto& aresta : arvore.arestas)
        arestas.adiciona_linha(
          { formata(grafo.get_rotulo(aresta.get_origem())),
            formata(grafo.get_rotulo(aresta.get_destino())),
            formata(aresta.get_peso()) });

      arestas.imprime();
    }

    total_arestas += arvore.arestas.size();
    resumo.adiciona_linha({ formata(i + 1),
                            formata(arvore.arestas.size()),
                            formata(arvore.custo_total) });
  }

  resumo.adiciona_separador();
  resumo.adiciona_linha(
    { "Total", formata(total_arestas), formata(floresta.custo_total) });

  std::cout << "\nResumo da floresta\n";
  resumo.imprime();
}

template<typename V, typename P, bool D>
void imprime_centralidades(const Grafo<V, P, D>& grafo)
{
  Tabela tabela(
    { { "Vertice" }, { "Centralidade", Tabela::Alinhamento::direita } });

  for (const auto& [vertice, centralidade] : grafo.computa_centralidades())
    tabela.adiciona_linha(
      { formata(grafo.get_rotulo(vertice)), formata(centralidade, 6) });

  tabela.imprime();
}

template<typename V, typename P, bool D>
void imprime_arvore_busca(const Grafo<V, P, D>& grafo,
                          const ArvoreBusca<P>& arvore,
                          int casas_custo)
{
  std::vector<std::string> ordem;
  for (vertice_id v : arvore.ordem_exploracao)
    ordem.push_back(formata(grafo.get_rotulo(v)));

  std::cout << "Raiz: " << formata(grafo.get_rotulo(arvore.raiz)) << '\n'
            << "Ordem de exploracao: " << junta(ordem, " -> ") << "\n\n";

  // filhos na ordem em que foram explorados
  std::vector<std::vector<vertice_id>> filhos(grafo.get_numero_vertices());
  for (vertice_id v : arvore.ordem_exploracao)
    if (arvore.pai[v])
      filhos[*arvore.pai[v]].push_back(v);

  std::cout << "Arvore (custo entre colchetes):\n";
  imprime_hierarquia(arvore.raiz, filhos, [&](vertice_id v) {
    return remove_acentos(formata(grafo.get_rotulo(v)) + " [" +
                          formata(*arvore.custo[v], casas_custo) + "]");
  });
  std::cout << '\n';

  Tabela tabela(
    { { "Vertice" }, { "Pai" }, { "Custo", Tabela::Alinhamento::direita } });

  for (vertice_id v = 0; v < grafo.get_numero_vertices(); ++v) {
    std::string pai =
      arvore.pai[v] ? formata(grafo.get_rotulo(*arvore.pai[v])) : "-";
    std::string custo =
      arvore.custo[v] ? formata(*arvore.custo[v], casas_custo) : "inalcancavel";

    tabela.adiciona_linha({ formata(grafo.get_rotulo(v)), pai, custo });
  }

  tabela.imprime();
}

template<typename V, typename P, bool D>
std::optional<V> escolhe_vertice(const Grafo<V, P, D>& grafo,
                                 const std::string& mensagem)
{
  const std::size_t n = grafo.get_numero_vertices();

  if (n == 0) {
    std::cout << "Grafo vazio: carregue um arquivo primeiro.\n";
    return std::nullopt;
  }

  // grade com quantas colunas couberem em ~80 caracteres
  std::vector<std::string> itens;
  std::size_t maior = 0;
  for (vertice_id v = 0; v < n; ++v) {
    itens.push_back(formata(grafo.get_rotulo(v)));
    maior = std::max(maior, itens.back().size());
  }

  const std::size_t largura_numero = formata(n).size();
  const std::size_t largura_item = 2 + largura_numero + 2 + maior + 2;
  const std::size_t colunas = std::max<std::size_t>(1, 80 / largura_item);

  std::cout << mensagem << "\n\n";
  for (std::size_t i = 0; i < n; ++i) {
    std::cout << "  " << std::right
              << std::setw(static_cast<int>(largura_numero)) << i + 1 << "  "
              << std::left << std::setw(static_cast<int>(maior + 2))
              << itens[i];
    if ((i + 1) % colunas == 0 || i + 1 == n)
      std::cout << '\n';
  }
  std::cout << "\n  " << std::right
            << std::setw(static_cast<int>(largura_numero)) << 0
            << "  Voltar\n\n";
  std::cout << std::left;

  while (true) {
    std::string linha = Entrada::le_texto("Numero ou nome do vertice: ");
    if (!std::cin) // fim da entrada (Ctrl+D)
      return std::nullopt;

    // o nome tem prioridade: num grafo com vértice "3", digitar 3 escolhe ele
    if constexpr (std::is_same_v<V, std::string>) {
      if (grafo.procura_vertice(linha))
        return linha;
    }

    std::istringstream conversor(linha);
    std::size_t escolha;
    if (conversor >> escolha && (conversor >> std::ws).eof() && escolha <= n) {
      if (escolha == 0)
        return std::nullopt;
      return grafo.get_rotulo(escolha - 1);
    }

    std::cout << "Vertice invalido: \"" << linha << "\"\n";
  }
}
