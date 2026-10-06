#include "vertice.hpp"
#include "teste.hpp"

#include <string>

TESTE(vertice_guarda_rotulo_string)
{
  Vertice<std::string> v("A");
  VERIFICA_IGUAL(v.get_rotulo(), std::string("A"));
}

TESTE(vertice_guarda_rotulo_int)
{
  Vertice<int> v(42);
  VERIFICA_IGUAL(v.get_rotulo(), 42);
}

TESTE(vertice_rotulo_e_copia_independente)
{
  std::string rotulo = "B";
  Vertice<std::string> v(rotulo);
  rotulo = "C";
  VERIFICA_IGUAL(v.get_rotulo(), std::string("B"));
}
