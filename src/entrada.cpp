#include "entrada.hpp"
#include "texto.hpp"

#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

namespace {
std::string le_linha(const std::string& mensagem)
{
  std::string entrada;

  std::cout << mensagem;
  std::getline(std::cin >> std::ws, entrada);

  return entrada;
}
}

namespace Entrada {
// sem acentos, para casar com os rotulos carregados do arquivo
std::string le_texto(const std::string& mensagem)
{
  return remove_acentos(le_linha(mensagem));
}

// o caminho fica intacto: uma pasta com acento precisa ser encontrada no disco
std::string le_caminho_arquivo(const std::string& mensagem)
{
  return le_linha(mensagem);
}

std::optional<std::string> escolhe_arquivo(const std::string& pasta)
{
  namespace fs = std::filesystem;

  // com error_code, pasta inexistente vira lista vazia em vez de exceção
  std::vector<fs::path> arquivos;
  std::error_code erro;
  for (const auto& item : fs::directory_iterator(pasta, erro))
    if (item.is_regular_file() && item.path().extension() == ".csv")
      arquivos.push_back(item.path());
  std::sort(arquivos.begin(), arquivos.end());

  if (arquivos.empty())
    std::cout << "Nenhum arquivo .csv encontrado em " << pasta << "/\n\n";
  else
    std::cout << "Arquivos em " << pasta << "/:\n\n";

  for (std::size_t i = 0; i < arquivos.size(); ++i)
    std::cout << std::right << std::setw(4) << i + 1 << "  " << std::left
              << arquivos[i].filename().string() << '\n';

  const std::size_t opcao_caminho = arquivos.size() + 1;
  std::cout << std::right << std::setw(4) << opcao_caminho << "  " << std::left
            << "Outro caminho...\n"
            << std::right << std::setw(4) << 0 << "  " << std::left
            << "Voltar\n\n";

  while (true) {
    std::string linha = le_linha("> ");
    if (!std::cin) // fim da entrada (Ctrl+D)
      return std::nullopt;

    std::istringstream conversor(linha);
    std::size_t escolha;

    if (conversor >> escolha && (conversor >> std::ws).eof() &&
        escolha <= opcao_caminho) {
      if (escolha == 0)
        return std::nullopt;
      if (escolha == opcao_caminho)
        return le_caminho_arquivo("Caminho do arquivo: ");
      return arquivos[escolha - 1].string();
    }

    std::cout << "Opcao invalida: \"" << linha << "\"\n";
  }
}
}
