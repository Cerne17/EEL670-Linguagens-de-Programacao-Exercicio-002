#include "grafo.hpp"
#include "grafo_util.hpp"
#include "teste.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using GrafoND = Grafo<std::string, double>;
using GrafoD = Grafo<std::string, double, true>;

// ---------- vértices ----------

TESTE(grafo_vazio_nao_tem_vertice)
{
  GrafoND g;
  VERIFICA(!g.procura_vertice("A"));
}

TESTE(grafo_ids_seguem_ordem_de_insercao)
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("B", "C");
  VERIFICA_IGUAL(g.get_rotulo(0), std::string("A"));
  VERIFICA_IGUAL(g.get_rotulo(1), std::string("B"));
  VERIFICA_IGUAL(g.get_rotulo(2), std::string("C"));
}

TESTE(grafo_vertice_repetido_nao_duplica)
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("A", "C");
  g.adiciona_aresta("B", "C");
  VERIFICA_IGUAL(g.get_numero_vertices(), 3u);
  VERIFICA_IGUAL(g.get_rotulo(2), std::string("C"));
}

TESTE(grafo_lista_vizinhos_id_invalido_lanca)
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  VERIFICA_LANCA(g.lista_vizinhos(99), std::out_of_range);
}

TESTE(grafo_get_id_segue_ordem_de_insercao)
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("B", "C");
  VERIFICA_IGUAL(g.get_id("A"), 0u);
  VERIFICA_IGUAL(g.get_id("B"), 1u);
  VERIFICA_IGUAL(g.get_id("C"), 2u);
}

TESTE(grafo_get_id_e_inverso_de_get_rotulo)
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("C", "A");
  for (std::size_t i = 0; i < g.get_numero_vertices(); ++i)
    VERIFICA_IGUAL(g.get_id(g.get_rotulo(i)), i);
}

TESTE(grafo_get_id_inexistente_lanca)
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  VERIFICA_LANCA(g.get_id("Z"), std::out_of_range);
}

// ---------- arestas ----------

TESTE(grafo_adiciona_aresta_cria_vertices_ausentes)
{
  GrafoND g;
  g.adiciona_aresta("A", "B", 2.2);
  VERIFICA(g.procura_vertice("A"));
  VERIFICA(g.procura_vertice("B"));
  VERIFICA(!g.procura_vertice("C"));
}

TESTE(grafo_nao_direcionado_liga_nos_dois_sentidos)
{
  GrafoND g;
  g.adiciona_aresta("A", "B", 2.2);
  std::size_t a = id_de(g, "A");
  std::size_t b = id_de(g, "B");

  VERIFICA_IGUAL(g.lista_vizinhos(a).size(), 1u);
  VERIFICA_IGUAL(g.lista_vizinhos(b).size(), 1u);
  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(a), b), 2.2);
  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(b), a), 2.2);
}

TESTE(grafo_direcionado_liga_em_um_sentido)
{
  GrafoD g;
  g.adiciona_aresta("A", "B", 2.2);
  std::size_t a = id_de(g, "A");
  std::size_t b = id_de(g, "B");

  VERIFICA_IGUAL(g.lista_vizinhos(a).size(), 1u);
  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(a), b), 2.2);
  VERIFICA(g.lista_vizinhos(b).empty());
}

TESTE(grafo_aresta_sem_peso_usa_peso_1)
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(id_de(g, "A")), id_de(g, "B")),
                   1.0);
}

// ---------- cenário de data/grafo_1.csv ----------

TESTE(grafo_cenario_grafo_1_csv)
{
  GrafoND g;
  g.adiciona_aresta("A", "B", 2.2);
  g.adiciona_aresta("B", "C", 1.0);
  g.adiciona_aresta("C", "D", 3.1);
  g.adiciona_aresta("A", "C", 1.2);

  std::size_t a = id_de(g, "A");
  std::size_t b = id_de(g, "B");
  std::size_t c = id_de(g, "C");
  std::size_t d = id_de(g, "D");

  VERIFICA_IGUAL(g.lista_vizinhos(a).size(), 2u);
  VERIFICA_IGUAL(g.lista_vizinhos(b).size(), 2u);
  VERIFICA_IGUAL(g.lista_vizinhos(c).size(), 3u);
  VERIFICA_IGUAL(g.lista_vizinhos(d).size(), 1u);

  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(a), b), 2.2);
  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(a), c), 1.2);
  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(c), d), 3.1);
  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(d), c), 3.1);
  VERIFICA_PROXIMO(peso_para(g.lista_vizinhos(b), d), -1.0);
}

// ---------- outros tipos de vértice ----------

TESTE(grafo_com_vertices_int)
{
  Grafo<int, int> g;
  g.adiciona_aresta(10, 20, 5);
  std::size_t v10 = id_de(g, 10);
  std::size_t v20 = id_de(g, 20);
  VERIFICA_IGUAL(g.lista_vizinhos(v10).size(), 1u);
  VERIFICA_IGUAL(g.lista_vizinhos(v10)[0].first, v20);
  VERIFICA_IGUAL(g.lista_vizinhos(v10)[0].second, 5);
}

// ---------- get_rotulo ----------

TESTE(grafo_get_rotulo_id_invalido_lanca)
{
  GrafoND g;
  VERIFICA_LANCA(g.get_rotulo(0), std::out_of_range);
}

// ---------- contadores ----------

TESTE(grafo_vazio_tem_zero_vertices_e_arestas)
{
  GrafoND g;
  VERIFICA_IGUAL(g.get_numero_vertices(), 0u);
  VERIFICA_IGUAL(g.get_numero_arestas(), 0u);
}

TESTE(grafo_nao_direcionado_conta_aresta_uma_vez)
{
  GrafoND g;
  g.adiciona_aresta("A", "B", 1.0);
  VERIFICA_IGUAL(g.get_numero_arestas(), 1u);
}

TESTE(grafo_direcionado_conta_cada_sentido)
{
  GrafoD g;
  g.adiciona_aresta("A", "B", 1.0);
  g.adiciona_aresta("B", "A", 1.0);
  VERIFICA_IGUAL(g.get_numero_arestas(), 2u);
}

// ---------- grau ----------

TESTE(grafo_grau_por_rotulo_e_por_id_coincidem)
{
  GrafoND g;
  g.adiciona_aresta("A", "B");
  g.adiciona_aresta("A", "C");
  std::size_t a = id_de(g, "A");
  VERIFICA_IGUAL(g.get_grau_por_rotulo("A"), 2u);
  VERIFICA_IGUAL(g.get_grau_por_id(a), 2u);
}

TESTE(grafo_direcionado_grau_e_de_saida)
{
  GrafoD g;
  g.adiciona_aresta("A", "B");
  VERIFICA_IGUAL(g.get_grau_por_rotulo("A"), 1u);
  VERIFICA_IGUAL(g.get_grau_por_rotulo("B"), 0u);
}

TESTE(grafo_grau_rotulo_inexistente_lanca)
{
  GrafoND g;
  VERIFICA_LANCA(g.get_grau_por_rotulo("Z"), std::out_of_range);
}

TESTE(grafo_grau_id_invalido_lanca)
{
  GrafoND g;
  VERIFICA_LANCA(g.get_grau_por_id(0), std::out_of_range);
}

TESTE(grafo_cenario_grafo_1_csv_contadores_e_graus)
{
  GrafoND g;
  g.adiciona_aresta("A", "B", 2.2);
  g.adiciona_aresta("B", "C", 1.0);
  g.adiciona_aresta("C", "D", 3.1);
  g.adiciona_aresta("A", "C", 1.2);

  VERIFICA_IGUAL(g.get_numero_vertices(), 4u);
  VERIFICA_IGUAL(g.get_numero_arestas(), 4u);
  VERIFICA_IGUAL(g.get_grau_por_rotulo("A"), 2u);
  VERIFICA_IGUAL(g.get_grau_por_rotulo("B"), 2u);
  VERIFICA_IGUAL(g.get_grau_por_rotulo("C"), 3u);
  VERIFICA_IGUAL(g.get_grau_por_rotulo("D"), 1u);
}

TESTE(grafo_int_grau_por_id_nao_confunde_com_rotulo)
{
  // rótulos int: o id 0 (vértice 10) não pode ser lido como rótulo 0
  Grafo<int, int> g;
  g.adiciona_aresta(10, 20);
  g.adiciona_aresta(10, 30);
  VERIFICA_IGUAL(g.get_grau_por_id(0), 2u);
  VERIFICA_IGUAL(g.get_grau_por_rotulo(10), 2u);
  VERIFICA_LANCA(g.get_grau_por_rotulo(0), std::out_of_range);
}

TESTE(grafo_vertices_size_t_compila)
{
  // rótulo e id com o mesmo tipo: as duas versões de grau devem coexistir
  Grafo<std::size_t, int> g;
  g.adiciona_aresta(5, 7);
  VERIFICA_IGUAL(g.get_grau_por_rotulo(5), 1u);
  VERIFICA_IGUAL(g.get_grau_por_id(0), 1u);
}

TESTE(grafo_peso_padrao_e_double)
{
  Grafo<std::string> g;
  g.adiciona_aresta("A", "B", 2.5);
  VERIFICA_PROXIMO(g.lista_vizinhos(0)[0].second, 2.5);
}
