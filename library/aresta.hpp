#pragma once

#include <cstddef>

template<typename Peso>
class Aresta
{
private:
  using id = std::size_t;
  id m_origem;
  id m_destino;
  Peso m_peso;

public:
  Aresta(id origem, id destino, const Peso& peso)
    : m_origem(origem)
    , m_destino(destino)
    , m_peso(peso) {};

  Aresta(id origem, id destino)
    : m_origem(origem)
    , m_destino(destino)
    , m_peso(Peso{ 1 }) {};

  id get_origem() const { return m_origem; };
  id get_destino() const { return m_destino; };
  const Peso& get_peso() const { return m_peso; };
};
