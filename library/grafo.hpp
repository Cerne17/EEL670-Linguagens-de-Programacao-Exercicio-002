#pragma once

#include "aresta.hpp"
#include "tipos.hpp"
#include "vertice.hpp"

#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

template<typename tipo_vertice,
         typename tipo_peso = double,
         bool Direcionado = false>
class Grafo
{
private:
  std::vector<Vertice<tipo_vertice>> m_vertices;
  std::unordered_map<tipo_vertice, vertice_id> m_mapeamento_vertices;
  std::vector<Aresta<tipo_peso>> m_arestas;
  std::vector<std::vector<std::pair<vertice_id, tipo_peso>>> m_adjacencias;

public:
  // true se estiver no mapeamento, false, caso contrario
  bool procura_vertice(const tipo_vertice& rotulo) const;

  void adiciona_aresta(const tipo_vertice& rotulo_origem,
                       const tipo_vertice& rotulo_destino,
                       const tipo_peso& peso);

  void adiciona_aresta(const tipo_vertice& rotulo_origem,
                       const tipo_vertice& rotulo_destino);

  const tipo_vertice& get_rotulo(vertice_id id_vertice) const;

  std::size_t get_numero_vertices() const;
  std::size_t get_numero_arestas() const;

  const std::vector<std::pair<vertice_id, tipo_peso>>& lista_vizinhos(
    vertice_id id_vertice) const;

  std::size_t get_grau_por_rotulo(const tipo_vertice& rotulo) const;
  std::size_t get_grau_por_id(vertice_id id_vertice) const;

  vertice_id get_id(const tipo_vertice& rotulo) const;

private:
  // registra um vertice em todos membros privados relevantes
  void cria_vertice(const tipo_vertice& rotulo);

public:
  /* ---- Algoritmos ---- */
  ArvoreBusca<tipo_peso> exploracao_bfs(const tipo_vertice& fonte) const;
  ArvoreBusca<tipo_peso> exploracao_dfs(const tipo_vertice& fonte) const;

  ComponentesConexas<vertice_id> get_componentes_conexas() const;

  FlorestaMST<tipo_peso> prim() const;
  FlorestaMST<tipo_peso> kruskal() const;
  /* ---- Algoritmos ---- */
};

// templates precisam da implementação
#include "grafo.tpp"
