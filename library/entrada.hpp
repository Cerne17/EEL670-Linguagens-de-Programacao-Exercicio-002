#pragma once

#include <optional>
#include <string>

namespace Entrada {
std::string le_texto(const std::string& mensagem);

std::string le_caminho_arquivo(const std::string& mensagem);

// lista os .csv de `pasta` e pede para escolher um deles ou digitar
// outro caminho; nullopt se o usuario escolher voltar
std::optional<std::string> escolhe_arquivo(const std::string& pasta);
}
