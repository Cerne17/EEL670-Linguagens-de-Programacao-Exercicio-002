#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <vector>

class Menu
{
public:
  using Acao = std::function<void()>;

  struct Opcao
  {
    std::string descricao;
    Acao acao;
  };

private:
  std::string m_titulo;
  std::vector<Opcao> m_opcoes;

public:
  explicit Menu(const std::string& titulo)
    : m_titulo(titulo) {};

  void adiciona_opcao(const std::string& descricao, Acao acao);

  void exibe() const;

  // laço principal: a cada opção, limpa a tela, executa a ação
  // e espera Enter antes de redesenhar o menu
  void executa();

private:
  // número da opção digitada, ou nullopt se a linha não for uma opção válida
  std::optional<std::size_t> interpreta_opcao(const std::string& linha) const;

  // largura das linhas horizontais do menu
  std::size_t largura() const;
};
