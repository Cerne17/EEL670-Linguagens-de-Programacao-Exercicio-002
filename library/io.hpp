#pragma once

#include "grafo.hpp"
#include "tipos.hpp"

#include <optional>
#include <string>

template<typename V, typename P, bool D>
void carrega_grafo(const std::string& caminho, Grafo<V, P, D>& grafo);

template<typename V, typename P, bool D>
void imprime_lista_adjacencia(const Grafo<V, P, D>& grafo);

template<typename V, typename P, bool D>
void imprime_estatisticas_grau(const Grafo<V, P, D>& grafo);

template<typename V, typename P, bool D>
void imprime_componentes(const Grafo<V, P, D>& grafo);

template<typename V, typename P, bool D>
void imprime_floresta_mst(const Grafo<V, P, D>& grafo,
                          const FlorestaMST<P>& floresta);

template<typename V, typename P, bool D>
void imprime_centralidades(const Grafo<V, P, D>& grafo);

// casas_custo: casas decimais do custo (0 para BFS/DFS, que contam arestas)
template<typename V, typename P, bool D>
void imprime_arvore_busca(const Grafo<V, P, D>& grafo,
                          const ArvoreBusca<P>& arvore,
                          int casas_custo = 2);

// desenho em ASCII com os vértices num círculo (até 20 vértices)
template<typename V, typename P, bool D>
void imprime_desenho_grafo(const Grafo<V, P, D>& grafo);

// matriz com o peso de cada aresta (até 15 vértices)
template<typename V, typename P, bool D>
void imprime_matriz_adjacencia(const Grafo<V, P, D>& grafo);

// lista os vértices numerados e pede um, por número ou por nome;
// nullopt se o grafo estiver vazio ou o usuário escolher voltar
template<typename V, typename P, bool D>
std::optional<V> escolhe_vertice(const Grafo<V, P, D>& grafo,
                                 const std::string& mensagem);

#include "io.tpp"
