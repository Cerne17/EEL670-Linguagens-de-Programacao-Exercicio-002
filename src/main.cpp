#include "entrada.hpp"
#include "grafo.hpp"
#include "io.hpp"
#include "menu.hpp"
#include "tabela.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

int main()
{
  using GrafoTrabalho = Grafo<std::string, double, false>;

  GrafoTrabalho grafo;

  std::string arquivo_atual;

  Menu menu("Laboratorio 6 - Grafos");

  /*
   * 1. Carregar arquivo
   */
  menu.adiciona_opcao("Carregar arquivo", [&grafo, &arquivo_atual]() {
    std::optional<std::string> escolhido = Entrada::escolhe_arquivo("data");

    if (!escolhido) // voltar
      return;

    const std::string& caminho = *escolhido;

    try {

      /*
       * Carregamos primeiro em um grafo temporario.
       *
       * Se houver erro no arquivo, o grafo atualmente
       * carregado continua intacto.
       */
      GrafoTrabalho novo_grafo;

      carrega_grafo(caminho, novo_grafo);

      grafo = std::move(novo_grafo);
      arquivo_atual = caminho;

      std::cout << "Arquivo carregado com sucesso.\n";
    } catch (const std::exception& erro) {
      std::cout << "Erro: " << erro.what() << '\n';
    }
  });

  /*
   * 2. Informacoes gerais
   */
  menu.adiciona_opcao("Informacoes do grafo", [&grafo, &arquivo_atual]() {
    Tabela tabela({ { "Propriedade" }, { "Valor" } });

    tabela.adiciona_linha(
      { "Arquivo", arquivo_atual.empty() ? "(nenhum)" : arquivo_atual });
    tabela.adiciona_linha(
      { "Vertices", std::to_string(grafo.get_numero_vertices()) });
    tabela.adiciona_linha(
      { "Arestas", std::to_string(grafo.get_numero_arestas()) });

    tabela.imprime();
  });

  /*
   * 3. Item 1 do trabalho
   */
  menu.adiciona_opcao("Lista de adjacencia",
                      [&grafo]() { imprime_lista_adjacencia(grafo); });

  /*
   * 4. Item 2 do trabalho
   */
  menu.adiciona_opcao("Estatisticas de grau",
                      [&grafo]() { imprime_estatisticas_grau(grafo); });

  /*
   * 5. Item 3
   */
  menu.adiciona_opcao("Componentes conexas",
                      [&grafo]() { imprime_componentes(grafo); });

  /*
   * 6. Prim
   */
  menu.adiciona_opcao("Floresta geradora minima - Prim", [&grafo]() {
    auto floresta = grafo.prim();

    imprime_floresta_mst(grafo, floresta);
  });

  /*
   * 7. Kruskal
   */
  menu.adiciona_opcao("Floresta geradora minima - Kruskal", [&grafo]() {
    auto floresta = grafo.kruskal();

    imprime_floresta_mst(grafo, floresta);
  });

  /*
   * 8. BFS
   */
  menu.adiciona_opcao("Executar BFS", [&grafo]() {
    std::optional<std::string> escolhido =
      escolhe_vertice(grafo, "Escolha o vertice fonte:");

    if (!escolhido) // voltar ou grafo vazio
      return;

    const std::string& fonte = *escolhido;

    try {
      std::cout << '\n';
      auto resultado = grafo.exploracao_bfs(fonte);

      imprime_arvore_busca(grafo, resultado, 0);
    } catch (const std::exception& erro) {
      std::cout << "Erro: " << erro.what() << '\n';
    }
  });

  /*
   * 9. DFS
   */
  menu.adiciona_opcao("Executar DFS", [&grafo]() {
    std::optional<std::string> escolhido =
      escolhe_vertice(grafo, "Escolha o vertice fonte:");

    if (!escolhido) // voltar ou grafo vazio
      return;

    const std::string& fonte = *escolhido;

    try {
      std::cout << '\n';
      auto resultado = grafo.exploracao_dfs(fonte);

      imprime_arvore_busca(grafo, resultado, 0);
    } catch (const std::exception& erro) {
      std::cout << "Erro: " << erro.what() << '\n';
    }
  });

  /*
   * 10. Dijkstra
   */
  menu.adiciona_opcao("Executar Dijkstra", [&grafo]() {
    std::optional<std::string> escolhido =
      escolhe_vertice(grafo, "Escolha o vertice fonte:");

    if (!escolhido) // voltar ou grafo vazio
      return;

    const std::string& fonte = *escolhido;

    try {
      std::cout << '\n';
      auto resultado = grafo.dijkstra(fonte);

      imprime_arvore_busca(grafo, resultado);
    } catch (const std::exception& erro) {
      std::cout << "Erro: " << erro.what() << '\n';
    }
  });

  /*
   * 11. Item 5
   */
  menu.adiciona_opcao("Centralidades de proximidade",
                      [&grafo]() { imprime_centralidades(grafo); });

  menu.executa();

  return 0;
}
