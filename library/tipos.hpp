#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "aresta.hpp"

using vertice_id = std::size_t;

template<typename P>
struct ArvoreBusca
{
  vertice_id raiz;

  std::vector<std::optional<vertice_id>> pai;
  std::vector<std::optional<P>> custo;
  std::vector<vertice_id> ordem_exploracao;
};

template<typename P>
struct MST
{
  std::vector<Aresta<P>> arestas;
  P custo_total{};
};

template<typename P>
struct FlorestaMST
{
  std::vector<MST<P>> arvores;
  P custo_total{};
};

template<typename ID>
struct ComponentesConexas
{
  std::vector<std::vector<ID>> componentes;

  std::size_t quantidade() const { return componentes.size(); };
};

class DisjointSet
{
private:
  std::vector<vertice_id> pai;
  std::vector<std::size_t> rank;

public:
  explicit DisjointSet(std::size_t n)
    : pai(n)
    , rank(n, 0)
  {
    for (vertice_id i = 0; i < n; ++i)
      pai[i] = i;
  }

  vertice_id find(vertice_id x)
  {
    if (pai[x] != x)
      pai[x] = find(pai[x]);

    return pai[x];
  }

  bool unite(vertice_id a, vertice_id b)
  {
    a = find(a);
    b = find(b);

    if (a == b)
      return false;

    if (rank[a] < rank[b])
      std::swap(a, b);

    pai[b] = a;

    if (rank[a] == rank[b])
      ++rank[a];

    return true;
  }
};
