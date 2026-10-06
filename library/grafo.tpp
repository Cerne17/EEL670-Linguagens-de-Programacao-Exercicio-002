#pragma once

#include "grafo.hpp"
#include "tipos.hpp"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <optional>
#include <queue>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

template<typename V, typename P, bool D>
bool Grafo<V, P, D>::procura_vertice(const V& rotulo) const
{
  auto iterador = m_mapeamento_vertices.find(rotulo);
  if (iterador != m_mapeamento_vertices.end()) { // encontrou
    return true;
  }
  return false;
}

template<typename V, typename P, bool D>
void Grafo<V, P, D>::adiciona_aresta(const V& rotulo_origem,
                                     const V& rotulo_destino,
                                     const P& peso)
{
  if (!this->procura_vertice(rotulo_origem))
    this->cria_vertice(rotulo_origem);
  if (!this->procura_vertice(rotulo_destino))
    this->cria_vertice(rotulo_destino);

  vertice_id origem = this->get_id(rotulo_origem);
  vertice_id destino = this->get_id(rotulo_destino);

  m_arestas.emplace_back(origem, destino, peso);
  m_adjacencias.at(origem).emplace_back(destino, peso);
  if constexpr (!D) {
    m_adjacencias.at(destino).emplace_back(origem, peso);
  }
}

template<typename V, typename P, bool D>
void Grafo<V, P, D>::adiciona_aresta(const V& rotulo_origem,
                                     const V& rotulo_destino)
{
  adiciona_aresta(rotulo_origem, rotulo_destino, P{ 1 });
}

template<typename V, typename P, bool D>
const V& Grafo<V, P, D>::get_rotulo(vertice_id id_vertice) const
{
  return m_vertices.at(id_vertice).get_rotulo();
}

template<typename V, typename P, bool D>
std::size_t Grafo<V, P, D>::get_numero_vertices() const
{
  return m_vertices.size();
}

template<typename V, typename P, bool D>
std::size_t Grafo<V, P, D>::get_numero_arestas() const
{
  return m_arestas.size();
}

template<typename V, typename P, bool D>
const std::vector<std::pair<vertice_id, P>>& Grafo<V, P, D>::lista_vizinhos(
  vertice_id id_vertice) const
{
  return m_adjacencias.at(id_vertice);
}

template<typename V, typename P, bool D>
std::size_t Grafo<V, P, D>::get_grau_por_rotulo(const V& rotulo) const
{
  return this->get_grau_por_id(this->get_id(rotulo));
}

template<typename V, typename P, bool D>
std::size_t Grafo<V, P, D>::get_grau_por_id(vertice_id id_vertice) const
{
  return m_adjacencias.at(id_vertice).size();
}

template<typename V, typename P, bool D>
void Grafo<V, P, D>::cria_vertice(const V& rotulo)
{
  if (this->procura_vertice(rotulo))
    return; // já existe

  vertice_id novo_id = m_vertices.size();
  m_vertices.emplace_back(rotulo); // cria o objeto no vetor
  m_mapeamento_vertices.emplace(rotulo, novo_id);
  m_adjacencias.emplace_back();
}

template<typename V, typename P, bool D>
vertice_id Grafo<V, P, D>::get_id(const V& rotulo) const
{
  auto iterador = m_mapeamento_vertices.find(rotulo);
  if (iterador == m_mapeamento_vertices.end())
    throw std::out_of_range("vertice nao encontrado no grafo");
  return iterador->second;
}
