#include "grafo.hpp"
#include "grafo_util.hpp"
#include "teste.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using GrafoND = Grafo<std::string, double>;
using GrafoD = Grafo<std::string, double, true>;
using Arvore = ArvoreBusca<double>;

// ---------- grafos de exemplo ----------

// data/grafo_1.csv
static GrafoND grafo_1()
{
  GrafoND g;
  g.adiciona_aresta("A", "B", 2.2);
  g.adiciona_aresta("B", "C", 1.0);
  g.adiciona_aresta("C", "D", 3.1);
  g.adiciona_aresta("A", "C", 1.2);
  return g;
}

// A - B - C - D
static GrafoND caminho()
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("B", "C");
  g.adiciona_aresta("C", "D");
  return g;
}

// A - B - C - D - A
static GrafoND ciclo_4()
{
  GrafoND g = caminho();
  g.adiciona_aresta("D", "A");
  return g;
}

// A - B - D
// |
// C
static GrafoND arvore()
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("A", "C");
  g.adiciona_aresta("B", "D");
  return g;
}

// A - B    C - D
static GrafoND desconexo()
{
  GrafoND g;
  g.adiciona_aresta("A", "B", 1.0);
  g.adiciona_aresta("C", "D", 2.0);
  return g;
}

// propriedades que toda árvore de busca precisa satisfazer
template<typename G>
static void verifica_arvore_valida(const G& g, const Arvore& arv)
{
  std::size_t n = g.get_numero_vertices();
  VERIFICA_IGUAL(arv.pai.size(), n);
  VERIFICA_IGUAL(arv.custo.size(), n);
  VERIFICA(!arv.ordem_exploracao.empty());
  VERIFICA_IGUAL(arv.ordem_exploracao.front(), arv.raiz);
  VERIFICA(!arv.pai.at(arv.raiz).has_value());
  VERIFICA_PROXIMO(custo_de(arv, arv.raiz), 0.0);

  std::vector<bool> visto(n, false);
  for (std::size_t v : arv.ordem_exploracao) {
    VERIFICA(!visto.at(v)); // cada vértice explorado uma vez
    visto.at(v) = true;
    VERIFICA(arv.custo.at(v).has_value());
    if (v != arv.raiz) {
      // o pai já foi explorado e existe a aresta pai -> v
      VERIFICA(arv.pai.at(v).has_value());
      std::size_t p = *arv.pai.at(v);
      VERIFICA(visto.at(p));
      VERIFICA(peso_para(g.lista_vizinhos(p), v) >= 0);
    }
  }

  for (std::size_t v = 0; v < n; ++v)
    if (!visto.at(v)) {
      VERIFICA(!arv.pai.at(v).has_value());
      VERIFICA(!arv.custo.at(v).has_value());
    }
}

// ---------- BFS ----------

TESTE(bfs_raiz_e_a_fonte)
{
  GrafoND g = grafo_1();
  Arvore arv = g.exploracao_bfs("B");
  VERIFICA_IGUAL(arv.raiz, id_de(g, "B"));
  verifica_arvore_valida(g, arv);
}

TESTE(bfs_caminho_explora_em_ordem)
{
  GrafoND g = caminho();
  Arvore arv = g.exploracao_bfs("A");
  std::vector<std::size_t> esperado = {
    id_de(g, "A"), id_de(g, "B"), id_de(g, "C"), id_de(g, "D")
  };
  VERIFICA(arv.ordem_exploracao == esperado);
  VERIFICA_IGUAL(arv.pai.at(id_de(g, "D")), id_de(g, "C"));
  VERIFICA_PROXIMO(custo_de(arv, id_de(g, "D")), 3.0);
}

TESTE(bfs_explora_por_niveis)
{
  GrafoND g = arvore();
  Arvore arv = g.exploracao_bfs("A");
  verifica_arvore_valida(g, arv);
  VERIFICA_IGUAL(arv.ordem_exploracao.back(), id_de(g, "D"));
  for (std::size_t i = 1; i < arv.ordem_exploracao.size(); ++i)
    VERIFICA(profundidade(arv, arv.ordem_exploracao[i - 1]) <=
             profundidade(arv, arv.ordem_exploracao[i]));
}

TESTE(bfs_ciclo_usa_menor_numero_de_arestas)
{
  GrafoND g = ciclo_4();
  Arvore arv = g.exploracao_bfs("A");
  verifica_arvore_valida(g, arv);
  VERIFICA_IGUAL(profundidade(arv, id_de(g, "B")), 1u);
  VERIFICA_IGUAL(profundidade(arv, id_de(g, "C")), 2u);
  VERIFICA_IGUAL(profundidade(arv, id_de(g, "D")), 1u);
}

TESTE(bfs_nao_alcanca_outra_componente)
{
  GrafoND g = desconexo();
  Arvore arv = g.exploracao_bfs("A");
  verifica_arvore_valida(g, arv);
  VERIFICA_IGUAL(arv.ordem_exploracao.size(), 2u);
  VERIFICA(!arv.pai.at(id_de(g, "C")).has_value());
  VERIFICA(!arv.custo.at(id_de(g, "D")).has_value());
}

TESTE(bfs_direcionado_segue_sentido_das_arestas)
{
  GrafoD g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("C", "A");
  Arvore arv = g.exploracao_bfs("A");
  verifica_arvore_valida(g, arv);
  VERIFICA_IGUAL(arv.ordem_exploracao.size(), 2u);
  VERIFICA(!arv.pai.at(id_de(g, "C")).has_value());
}

TESTE(bfs_fonte_inexistente_lanca)
{
  GrafoND g = grafo_1();
  VERIFICA_LANCA(g.exploracao_bfs("Z"), std::out_of_range);
}

// ---------- DFS ----------

TESTE(dfs_raiz_e_a_fonte)
{
  GrafoND g = grafo_1();
  Arvore arv = g.exploracao_dfs("C");
  VERIFICA_IGUAL(arv.raiz, id_de(g, "C"));
  verifica_arvore_valida(g, arv);
}

TESTE(dfs_caminho_explora_em_ordem)
{
  GrafoND g = caminho();
  Arvore arv = g.exploracao_dfs("A");
  std::vector<std::size_t> esperado = {
    id_de(g, "A"), id_de(g, "B"), id_de(g, "C"), id_de(g, "D")
  };
  VERIFICA(arv.ordem_exploracao == esperado);
}

TESTE(dfs_ciclo_vai_fundo)
{
  // no ciclo de 4, a DFS percorre o ciclo inteiro antes de voltar:
  // a árvore vira um caminho com profundidade 3 (a BFS teria no máximo 2)
  GrafoND g = ciclo_4();
  Arvore arv = g.exploracao_dfs("A");
  verifica_arvore_valida(g, arv);
  std::size_t maior = 0;
  for (std::size_t v : arv.ordem_exploracao)
    maior = std::max(maior, profundidade(arv, v));
  VERIFICA_IGUAL(maior, 3u);
}

TESTE(dfs_nao_alcanca_outra_componente)
{
  GrafoND g = desconexo();
  Arvore arv = g.exploracao_dfs("C");
  verifica_arvore_valida(g, arv);
  VERIFICA_IGUAL(arv.ordem_exploracao.size(), 2u);
  VERIFICA(!arv.pai.at(id_de(g, "A")).has_value());
}

TESTE(dfs_direcionado_segue_sentido_das_arestas)
{
  GrafoD g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("C", "A");
  Arvore arv = g.exploracao_dfs("B");
  verifica_arvore_valida(g, arv);
  VERIFICA_IGUAL(arv.ordem_exploracao.size(), 1u);
}

TESTE(dfs_fonte_inexistente_lanca)
{
  GrafoND g = grafo_1();
  VERIFICA_LANCA(g.exploracao_dfs("Z"), std::out_of_range);
}

// ---------- componentes conexas ----------

// ordena cada componente e a lista, para comparar sem depender da ordem
static std::vector<std::vector<std::size_t>> normaliza(
  std::vector<std::vector<std::size_t>> componentes)
{
  for (auto& c : componentes)
    std::sort(c.begin(), c.end());
  std::sort(componentes.begin(), componentes.end());
  return componentes;
}

TESTE(componentes_grafo_vazio)
{
  GrafoND g;
  VERIFICA_IGUAL(g.get_componentes_conexas().quantidade(), 0u);
}

TESTE(componentes_grafo_conexo_tem_uma)
{
  GrafoND g = grafo_1();
  auto cc = g.get_componentes_conexas();
  VERIFICA_IGUAL(cc.quantidade(), 1u);
  VERIFICA_IGUAL(cc.componentes.at(0).size(), 4u);
}

TESTE(componentes_grafo_desconexo)
{
  GrafoND g = desconexo();
  g.adiciona_aresta("E", "F");
  auto cc = g.get_componentes_conexas();
  VERIFICA_IGUAL(cc.quantidade(), 3u);

  std::vector<std::vector<std::size_t>> esperado = {
    { id_de(g, "A"), id_de(g, "B") },
    { id_de(g, "C"), id_de(g, "D") },
    { id_de(g, "E"), id_de(g, "F") },
  };
  VERIFICA(normaliza(cc.componentes) == normaliza(esperado));
}

// ---------- árvore geradora mínima (Prim e Kruskal) ----------

// arestas da MST como pares (menor id, maior id), ordenados
static std::vector<std::pair<std::size_t, std::size_t>> pares(
  const MST<double>& mst)
{
  std::vector<std::pair<std::size_t, std::size_t>> resultado;
  for (const auto& a : mst.arestas)
    resultado.push_back({ std::min(a.get_origem(), a.get_destino()),
                          std::max(a.get_origem(), a.get_destino()) });
  std::sort(resultado.begin(), resultado.end());
  return resultado;
}

static double soma_pesos(const MST<double>& mst)
{
  double soma = 0;
  for (const auto& a : mst.arestas)
    soma += a.get_peso();
  return soma;
}

// exemplo clássico com 6 vértices e MST de custo 14
static GrafoND grafo_6()
{
  GrafoND g;
  g.adiciona_aresta("A", "B", 4);
  g.adiciona_aresta("A", "C", 4);
  g.adiciona_aresta("B", "C", 2);
  g.adiciona_aresta("C", "D", 3);
  g.adiciona_aresta("C", "F", 4);
  g.adiciona_aresta("C", "E", 2);
  g.adiciona_aresta("D", "F", 3);
  g.adiciona_aresta("E", "F", 3);
  return g;
}

// mesmas verificações para os dois algoritmos
static void verifica_mst_grafo_1(const GrafoND& g, const FlorestaMST<double>& f)
{
  VERIFICA_IGUAL(f.arvores.size(), 1u);
  VERIFICA_PROXIMO(f.custo_total, 5.3);

  const MST<double>& mst = f.arvores.at(0);
  VERIFICA_PROXIMO(mst.custo_total, 5.3);
  VERIFICA_PROXIMO(soma_pesos(mst), 5.3);

  std::size_t a = id_de(g, "A"), b = id_de(g, "B");
  std::size_t c = id_de(g, "C"), d = id_de(g, "D");
  std::vector<std::pair<std::size_t, std::size_t>> esperado = {
    { std::min(a, c), std::max(a, c) },
    { std::min(b, c), std::max(b, c) },
    { std::min(c, d), std::max(c, d) },
  };
  std::sort(esperado.begin(), esperado.end());
  VERIFICA(pares(mst) == esperado);
}

static void verifica_floresta_desconexo(const FlorestaMST<double>& f)
{
  VERIFICA_IGUAL(f.arvores.size(), 2u);
  VERIFICA_PROXIMO(f.custo_total, 3.0);
  for (const auto& mst : f.arvores) {
    VERIFICA_IGUAL(mst.arestas.size(), 1u);
    VERIFICA_PROXIMO(mst.custo_total, soma_pesos(mst));
  }
}

TESTE(prim_grafo_1)
{
  GrafoND g = grafo_1();
  verifica_mst_grafo_1(g, g.prim());
}

TESTE(kruskal_grafo_1)
{
  GrafoND g = grafo_1();
  verifica_mst_grafo_1(g, g.kruskal());
}

TESTE(prim_e_kruskal_tem_mesmo_custo)
{
  GrafoND g = grafo_6();
  FlorestaMST<double> p = g.prim();
  FlorestaMST<double> k = g.kruskal();
  VERIFICA_PROXIMO(p.custo_total, 14.0);
  VERIFICA_PROXIMO(k.custo_total, 14.0);
  VERIFICA_IGUAL(p.arvores.at(0).arestas.size(), 5u);
  VERIFICA_IGUAL(k.arvores.at(0).arestas.size(), 5u);
}

TESTE(prim_grafo_desconexo_gera_floresta)
{
  verifica_floresta_desconexo(desconexo().prim());
}

TESTE(kruskal_grafo_desconexo_gera_floresta)
{
  verifica_floresta_desconexo(desconexo().kruskal());
}

TESTE(mst_grafo_vazio)
{
  GrafoND g;
  VERIFICA(g.prim().arvores.empty());
  VERIFICA(g.kruskal().arvores.empty());
  VERIFICA_PROXIMO(g.prim().custo_total, 0.0);
  VERIFICA_PROXIMO(g.kruskal().custo_total, 0.0);
}
