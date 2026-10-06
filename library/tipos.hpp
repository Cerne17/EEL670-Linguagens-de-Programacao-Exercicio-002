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