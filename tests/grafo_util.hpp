#pragma once

// Funções auxiliares compartilhadas pelos testes de Grafo.

#include <cstddef>
#include <utility>
#include <vector>

// retorna o peso da aresta para `destino` em `vizinhos`, ou -1 se não houver
template<typename P>
double peso_para(const std::vector<std::pair<std::size_t, P>>& vizinhos,
                 std::size_t destino)
{
  for (const auto& par : vizinhos)
    if (par.first == destino)
      return par.second;
  return -1;
}

// atalho para grafo.get_id(rotulo), aceitando literais como "A"
template<typename G, typename R>
std::size_t id_de(const G& grafo, const R& rotulo)
{
  return grafo.get_id(rotulo);
}

// custo de `v` na árvore, ou -1 se `v` não foi alcançado
template<typename Arvore>
double custo_de(const Arvore& arvore, std::size_t v)
{
  return arvore.custo.at(v) ? static_cast<double>(*arvore.custo.at(v)) : -1;
}

// número de arestas entre `v` e a raiz, seguindo `pai`
template<typename Arvore>
std::size_t profundidade(const Arvore& arvore, std::size_t v)
{
  std::size_t passos = 0;
  while (arvore.pai.at(v)) {
    v = *arvore.pai.at(v);
    ++passos;
  }
  return passos;
}
