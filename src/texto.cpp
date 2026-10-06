#include "texto.hpp"

#include <cstddef>

std::string remove_acentos(const std::string& texto)
{
  // substitutos para U+00C0 .. U+00FF, na ordem da tabela Unicode
  // (À Á Â Ã Ä Å Æ Ç È É ... ý þ ÿ)
  static const char substitutos[] = "AAAAAAAC"
                                    "EEEEIIII"
                                    "DNOOOOOx"
                                    "OUUUUYTs"
                                    "aaaaaaac"
                                    "eeeeiiii"
                                    "dnooooo/"
                                    "ouuuuyty";

  std::string resultado;
  resultado.reserve(texto.size());

  for (std::size_t i = 0; i < texto.size(); ++i) {
    unsigned char byte = texto[i];
    unsigned char proximo =
      i + 1 < texto.size() ? static_cast<unsigned char>(texto[i + 1]) : 0;

    // em UTF-8, U+00C0..U+00FF são codificados como 0xC3 0x80..0xBF
    if (byte == 0xC3 && proximo >= 0x80 && proximo <= 0xBF) {
      resultado += substitutos[proximo - 0x80];
      ++i;
    } else if (byte == 0xC2 && proximo == 0xAA) { // ª
      resultado += 'a';
      ++i;
    } else if (byte == 0xC2 && proximo == 0xBA) { // º
      resultado += 'o';
      ++i;
    } else {
      resultado += texto[i];
    }
  }

  return resultado;
}
