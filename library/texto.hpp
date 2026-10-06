#pragma once

#include <string>

// Troca letras acentuadas (UTF-8, faixa Latin-1) pela versão sem acento:
// "João" -> "Joao", "Ação" -> "Acao", "1º" -> "1o".
// Outros caracteres não-ASCII são mantidos como estão.
std::string remove_acentos(const std::string& texto);
