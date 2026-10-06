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

template<typename V, typename P, bool D>
ArvoreBusca<P> Grafo<V, P, D>::exploracao_bfs(const V& fonte) const
{
  vertice_id id_fonte = this->get_id(fonte);

  ArvoreBusca<P> resultado;
  resultado.raiz = id_fonte;

  const std::size_t n = m_vertices.size();

  resultado.pai.resize(n);
  resultado.custo.resize(n);

  resultado.custo[id_fonte] = P{};

  std::queue<vertice_id> fila;
  fila.push(id_fonte);

  while (!fila.empty()) {
    vertice_id atual = fila.front();
    fila.pop();

    resultado.ordem_exploracao.push_back(atual);

    for (const auto& adjacente : m_adjacencias.at(atual)) {
      vertice_id vizinho = adjacente.first;

      if (!resultado.custo[vizinho].has_value()) {
        resultado.pai[vizinho] = atual;
        resultado.custo[vizinho] = resultado.custo[atual].value() + P{ 1 };
        fila.push(vizinho);
      }
    }
  }
  return resultado;
}

template<typename V, typename P, bool D>
ArvoreBusca<P> Grafo<V, P, D>::exploracao_dfs(const V& fonte) const
{
  vertice_id id_fonte = this->get_id(fonte);

  ArvoreBusca<P> resultado;
  resultado.raiz = id_fonte;

  const std::size_t n = m_vertices.size();

  resultado.pai.resize(n);
  resultado.custo.resize(n);

  resultado.custo[id_fonte] = P{};

  std::vector<bool> visitado(n, false);

  // cada item é (vértice, pai): o vértice só é marcado ao sair da pilha,
  // senão todos os vizinhos ganhariam o mesmo pai e a busca viraria uma BFS
  std::stack<std::pair<vertice_id, std::optional<vertice_id>>> pilha;
  pilha.push({ id_fonte, std::nullopt });

  while (!pilha.empty()) {
    auto [atual, pai] = pilha.top();
    pilha.pop();

    if (visitado[atual]) // empilhado mais de uma vez; já explorado
      continue;
    visitado[atual] = true;

    if (pai) {
      resultado.pai[atual] = pai;
      resultado.custo[atual] = resultado.custo[*pai].value() + P{ 1 };
    }
    resultado.ordem_exploracao.push_back(atual);

    // empilha de trás para frente: o primeiro vizinho sai primeiro
    const auto& vizinhos = m_adjacencias.at(atual);
    for (auto it = vizinhos.rbegin(); it != vizinhos.rend(); ++it)
      if (!visitado[it->first])
        pilha.push({ it->first, atual });
  }
  return resultado;
}

template<typename V, typename P, bool D>
ComponentesConexas<vertice_id> Grafo<V, P, D>::get_componentes_conexas() const
{
  static_assert(!D, "get_componentes_conexas() requer grafo nao direcionado");

  ComponentesConexas<vertice_id> resultado;
  std::vector<bool> visitado(m_vertices.size(), false);

  for (vertice_id inicio = 0; inicio < m_vertices.size(); inicio++) {
    if (visitado[inicio])
      continue;

    std::vector<vertice_id> componente;
    std::queue<vertice_id> fila;

    fila.push(inicio);
    visitado[inicio] = true;

    while (!fila.empty()) {
      vertice_id atual = fila.front();
      fila.pop();

      componente.push_back(atual);

      for (const auto& adjacente : m_adjacencias.at(atual)) {
        vertice_id vizinho = adjacente.first;

        if (!visitado[vizinho]) {
          visitado[vizinho] = true;
          fila.push(vizinho);
        }
      }
    }
    resultado.componentes.push_back(componente);
  }
  return resultado;
}

template<typename V, typename P, bool D>
FlorestaMST<P> Grafo<V, P, D>::prim() const
{
  static_assert(!D, "Prim requer um grafo nao direcionado");

  FlorestaMST<P> floresta;

  std::vector<bool> visitado(m_vertices.size(), false);

  using entrada = std::tuple<P, vertice_id, vertice_id>;

  for (vertice_id inicio = 0; inicio < m_vertices.size(); ++inicio) {

    if (visitado[inicio])
      continue;

    MST<P> arvore;

    std::priority_queue<entrada, std::vector<entrada>, std::greater<entrada>>
      fila;

    visitado[inicio] = true;

    for (const auto& [vizinho, peso] : m_adjacencias.at(inicio)) {
      fila.emplace(peso, inicio, vizinho);
    }

    while (!fila.empty()) {

      auto [peso, origem, destino] = fila.top();
      fila.pop();

      if (visitado[destino])
        continue;

      visitado[destino] = true;

      arvore.arestas.emplace_back(origem, destino, peso);
      arvore.custo_total += peso;
      floresta.custo_total += peso;

      for (const auto& [vizinho, peso_aresta] : m_adjacencias.at(destino)) {

        if (!visitado[vizinho]) {
          fila.emplace(peso_aresta, destino, vizinho);
        }
      }
    }

    floresta.arvores.push_back(std::move(arvore));
  }

  return floresta;
}

template<typename V, typename P, bool D>
FlorestaMST<P> Grafo<V, P, D>::kruskal() const
{
  static_assert(!D, "Kruskal requer um grafo nao direcionado");

  FlorestaMST<P> floresta;

  std::vector<Aresta<P>> arestas_ordenadas = m_arestas;

  std::sort(
    arestas_ordenadas.begin(),
    arestas_ordenadas.end(),
    [](const auto& a, const auto& b) { return a.get_peso() < b.get_peso(); });

  DisjointSet dsu(m_vertices.size());

  std::vector<Aresta<P>> escolhidas;

  for (const auto& aresta : arestas_ordenadas) {

    vertice_id u = aresta.get_origem();
    vertice_id v = aresta.get_destino();

    if (dsu.unite(u, v)) {
      escolhidas.push_back(aresta);
      floresta.custo_total += aresta.get_peso();
    }
  }

  // separar as arestas escolhidas por componente
  std::unordered_map<vertice_id, MST<P>> componentes;

  for (const auto& aresta : escolhidas) {

    vertice_id raiz = dsu.find(aresta.get_origem());

    componentes[raiz].arestas.push_back(aresta);
    componentes[raiz].custo_total += aresta.get_peso();
  }

  // inclui também componentes formadas por vértices isolados
  for (vertice_id v = 0; v < m_vertices.size(); ++v) {
    vertice_id raiz = dsu.find(v);

    if (!componentes.contains(raiz)) {
      componentes.emplace(raiz, MST<P>{});
    }
  }

  for (auto& [_, arvore] : componentes) {
    floresta.arvores.push_back(std::move(arvore));
  }

  return floresta;
}

template<typename V, typename P, bool D>
ArvoreBusca<P> Grafo<V, P, D>::dijkstra(const V& fonte) const
{
  vertice_id id_fonte = this->get_id(fonte);

  ArvoreBusca<P> resultado;
  resultado.raiz = id_fonte;

  const std::size_t n = m_vertices.size();

  resultado.pai.resize(n);
  resultado.custo.resize(n);

  using tipo_fila = std::pair<P, vertice_id>;

  std::
    priority_queue<tipo_fila, std::vector<tipo_fila>, std::greater<tipo_fila>>
      fila;

  resultado.custo[id_fonte] = P{};

  fila.emplace(P{}, id_fonte);

  while (!fila.empty()) {
    auto [custo_atual, atual] = fila.top();
    fila.pop();

    // já tem caminho mais barato até o vertice atual -> ignora este caminho
    if (resultado.custo[atual].has_value() &&
        custo_atual > resultado.custo[atual].value())
      continue;

    resultado.ordem_exploracao.push_back(atual);

    for (const auto& [vizinho, peso] : m_adjacencias.at(atual)) {

      P novo_custo = custo_atual + peso;

      if (!resultado.custo[vizinho].has_value() ||
          novo_custo < resultado.custo[vizinho].value()) {

        resultado.custo[vizinho] = novo_custo;
        resultado.pai[vizinho] = atual;

        fila.emplace(novo_custo, vizinho);
      }
    }
  }
  return resultado;
}
