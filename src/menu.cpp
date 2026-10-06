#include "menu.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

namespace {

// sequência ANSI: apaga a tela e leva o cursor ao canto superior esquerdo
void limpa_tela()
{
  std::cout << "\033[2J\033[H" << std::flush;
}

void imprime_cabecalho(const std::string& texto, std::size_t largura)
{
  std::cout << std::string(largura, '=') << '\n'
            << "  " << texto << '\n'
            << std::string(largura, '=') << "\n\n";
}

// segura o resultado na tela até o usuário apertar Enter
void pausa()
{
  std::cout << "\nPressione Enter para voltar ao menu...";
  std::string descarte;
  std::getline(std::cin, descarte);
}

} // namespace

void Menu::adiciona_opcao(const std::string& descricao, Acao acao)
{
  m_opcoes.push_back({ descricao, std::move(acao) });
}

std::size_t Menu::largura() const
{
  std::size_t maior = m_titulo.size();
  for (const Opcao& opcao : m_opcoes)
    maior = std::max(maior, opcao.descricao.size());

  // 4 do número + 2 de espaço antes da descrição + 2 de folga
  return std::max<std::size_t>(maior + 8, 40);
}

void Menu::exibe() const
{
  imprime_cabecalho(m_titulo, largura());

  for (std::size_t i = 0; i < m_opcoes.size(); ++i) {
    std::cout << std::right << std::setw(4) << i + 1 << "  " << std::left
              << m_opcoes[i].descricao << '\n';
  }

  std::cout << std::right << std::setw(4) << 0 << "  " << std::left << "Sair\n"
            << std::string(largura(), '-') << '\n';
}

std::optional<std::size_t> Menu::interpreta_opcao(
  const std::string& linha) const
{
  std::istringstream entrada(linha);
  std::size_t escolha;

  // recusa texto, números negativos e lixo depois do número ("3a")
  if (!(entrada >> escolha) || !(entrada >> std::ws).eof())
    return std::nullopt;

  if (escolha > m_opcoes.size())
    return std::nullopt;

  return escolha;
}

void Menu::executa()
{
  std::string aviso;

  while (true) {
    limpa_tela();
    exibe();

    if (!aviso.empty()) {
      std::cout << aviso << '\n';
      aviso.clear();
    }

    std::cout << "> ";

    // lê a linha inteira para não deixar lixo no buffer para as ações
    std::string linha;
    if (!std::getline(std::cin, linha)) // fim da entrada (Ctrl+D)
      return;

    std::optional<std::size_t> escolha = interpreta_opcao(linha);

    if (!escolha) {
      aviso = "Opcao invalida: \"" + linha + "\"";
      continue;
    }

    if (*escolha == 0) {
      limpa_tela();
      return;
    }

    const Opcao& opcao = m_opcoes[*escolha - 1];

    limpa_tela();
    imprime_cabecalho(opcao.descricao, largura());
    opcao.acao();
    pausa();
  }
}
