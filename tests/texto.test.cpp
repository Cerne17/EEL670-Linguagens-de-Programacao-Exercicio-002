#include "teste.hpp"
#include "texto.hpp"

#include <string>

TESTE(texto_sem_acento_fica_igual)
{
  VERIFICA_IGUAL(remove_acentos("Miguel, Ana"), std::string("Miguel, Ana"));
  VERIFICA_IGUAL(remove_acentos(""), std::string(""));
}

TESTE(texto_remove_acentos_minusculos)
{
  VERIFICA_IGUAL(remove_acentos("joão"), std::string("joao"));
  VERIFICA_IGUAL(remove_acentos("ação"), std::string("acao"));
  VERIFICA_IGUAL(remove_acentos("àéîõü"), std::string("aeiou"));
}

TESTE(texto_remove_acentos_maiusculos)
{
  VERIFICA_IGUAL(remove_acentos("JOÃO"), std::string("JOAO"));
  VERIFICA_IGUAL(remove_acentos("ÁÉÍÓÚ"), std::string("AEIOU"));
  VERIFICA_IGUAL(remove_acentos("Ç"), std::string("C"));
}

TESTE(texto_remove_ordinais)
{
  VERIFICA_IGUAL(remove_acentos("1º e 2ª"), std::string("1o e 2a"));
}

TESTE(texto_resultado_tem_um_byte_por_letra)
{
  // é isso que mantém as colunas alinhadas com setw
  VERIFICA_IGUAL(remove_acentos("João").size(), 4u);
}

TESTE(texto_mantem_outros_caracteres_nao_ascii)
{
  VERIFICA_IGUAL(remove_acentos("€"), std::string("€"));
}
