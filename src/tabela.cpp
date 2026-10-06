#include "tabela.hpp"
#include "texto.hpp"

#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <utility>

Tabela::Tabela(std::vector<Coluna> colunas)
  : m_colunas(std::move(colunas))
{
  // setw conta bytes: sem acentos, cada letra ocupa uma coluna
  for (Coluna& coluna : m_colunas)
    coluna.titulo = remove_acentos(coluna.titulo);
}

void Tabela::adiciona_linha(std::vector<std::string> celulas)
{
  if (celulas.size() != m_colunas.size())
    throw std::invalid_argument(
      "Tabela: numero de celulas diferente do numero de colunas");

  for (std::string& celula : celulas)
    celula = remove_acentos(celula);

  m_linhas.push_back(std::move(celulas));
}

void Tabela::adiciona_separador()
{
  m_linhas.emplace_back();
}

void Tabela::imprime(std::ostream& saida) const
{
  // guarda a formatação do stream para não vazar left/right para o resto
  std::ios_base::fmtflags formato_original = saida.flags();

  const std::vector<std::size_t> l = larguras();

  std::vector<std::string> titulos;
  for (const Coluna& coluna : m_colunas)
    titulos.push_back(coluna.titulo);

  imprime_separador(saida, l);
  imprime_celulas(saida, titulos, l);
  imprime_separador(saida, l);

  for (const auto& linha : m_linhas) {
    if (linha.empty())
      imprime_separador(saida, l);
    else
      imprime_celulas(saida, linha, l);
  }

  imprime_separador(saida, l);

  saida.flags(formato_original);
}

std::vector<std::size_t> Tabela::larguras() const
{
  std::vector<std::size_t> resultado;

  for (const Coluna& coluna : m_colunas)
    resultado.push_back(coluna.titulo.size());

  for (const auto& linha : m_linhas)
    for (std::size_t i = 0; i < linha.size(); ++i)
      resultado[i] = std::max(resultado[i], linha[i].size());

  return resultado;
}

void Tabela::imprime_separador(std::ostream& saida,
                               const std::vector<std::size_t>& larguras) const
{
  saida << '+';
  for (std::size_t largura : larguras)
    saida << std::string(largura + 2, '-') << '+';
  saida << '\n';
}

void Tabela::imprime_celulas(std::ostream& saida,
                             const std::vector<std::string>& celulas,
                             const std::vector<std::size_t>& larguras) const
{
  saida << '|';

  for (std::size_t i = 0; i < celulas.size(); ++i) {
    if (m_colunas[i].alinhamento == Alinhamento::direita)
      saida << std::right;
    else
      saida << std::left;

    saida << ' ' << std::setw(static_cast<int>(larguras[i])) << celulas[i]
          << " |";
  }

  saida << '\n';
}
