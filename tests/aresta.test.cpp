#include "aresta.hpp"
#include "teste.hpp"

#include <string>

TESTE(aresta_guarda_origem_e_destino)
{
  Aresta<double> a(0, 1, 2.2);
  VERIFICA_IGUAL(a.get_origem(), 0u);
  VERIFICA_IGUAL(a.get_destino(), 1u);
}

TESTE(aresta_guarda_peso_double)
{
  // grafo_1.csv: A,B,2.2
  Aresta<double> a(0, 1, 2.2);
  VERIFICA_PROXIMO(a.get_peso(), 2.2);
}

TESTE(aresta_guarda_peso_int)
{
  Aresta<int> a(2, 3, 7);
  VERIFICA_IGUAL(a.get_peso(), 7);
}

TESTE(aresta_com_peso_padrao)
{
  Aresta<double> a(2, 3);
  VERIFICA_PROXIMO(a.get_peso(), 1.0);
}
TESTE(aresta_de_string_com_peso_padrao)
{
  Aresta<std::string> a(2, 3);
  VERIFICA_IGUAL(a.get_peso().size(), 1);
}

TESTE(aresta_com_peso_string)
{
  Aresta<std::string> a(2, 3, "abacate");
  VERIFICA_IGUAL(a.get_peso(), "abacate");
}
