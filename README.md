# Grafos

Miguel Badany Cerne — DRE: 123370433

Biblioteca de grafos ponderados em C++ com buscas, componentes conexas,
florestas geradoras mínimas, caminhos mínimos e centralidade de proximidade,
operada por um menu de console.

## Compilar e executar

Requer GCC ou Clang com suporte a C++20 e Make. Na pasta do projeto:

```sh
make
make run
```

`make test` compila e executa os testes unitários.
`make clean` remove o executável, os testes e os objetos de `out/`.

Execute a partir da raiz do projeto: a opção de carregar arquivo lista os
`.csv` de `./data`.

## Estrutura

| Pasta | Conteúdo |
|---|---|
| `library/` | Cabeçalhos; `grafo.tpp` e `io.tpp` trazem a implementação dos templates |
| `src/` | Programa principal, menu, entrada, tabela e remoção de acentos |
| `tests/` | Testes unitários com um mini framework próprio (`teste.hpp`) |
| `data/` | Grafos de exemplo em CSV |

A classe `Grafo<tipo_vertice, tipo_peso, Direcionado>` guarda o grafo em lista
de adjacência. O programa usa `Grafo<std::string, double>`, não direcionado.

## Menu

| Opção | Operação |
|---|---|
| 1 | Carregar arquivo de `data/` ou de um caminho informado |
| 2 | Exibir arquivo, número de vértices e de arestas |
| 3 | Exibir lista de adjacência com graus e pesos |
| 4 | Exibir vértices de maior e menor grau e grau médio |
| 5 | Exibir componentes conexas |
| 6 | Floresta geradora mínima por Prim |
| 7 | Floresta geradora mínima por Kruskal |
| 8 | Busca em largura (BFS) a partir de um vértice |
| 9 | Busca em profundidade (DFS) a partir de um vértice |
| 10 | Caminhos mínimos por Dijkstra a partir de um vértice |
| 11 | Centralidade de proximidade de todos os vértices |
| 12 | Desenho do grafo em ASCII e matriz de adjacência |
| 0 | Sair |

A tela é limpa a cada opção; Enter volta ao menu. Nas opções 8 a 10, o vértice
fonte é escolhido pelo número na lista ou pelo nome.

- **Opções 6 e 7:** em grafos desconexos, geram uma árvore por componente.
- **Opções 8 a 10:** exibem a árvore em hierarquia e a tabela de pais e custos.
  Em BFS e DFS o custo é o número de arestas; em Dijkstra, a soma dos pesos.
- **Opção 11:** usa `c(v) = 1 / Σ d(v, t)`, com as distâncias de Dijkstra.
  Vértices inalcançáveis ficam fora da soma, então o valor só é comparável
  entre grafos conexos.
- **Opção 12:** o desenho aceita até 20 vértices e a matriz até 15.

## Arquivos de entrada

Uma aresta por linha, no formato `origem,destino,peso`, com ponto para
decimais:

```
A,B,2.2
B,C,1.0
```

Acentos são removidos dos rótulos ao carregar e do que é digitado, para que as
colunas das tabelas fiquem alinhadas; `João` vira `Joao`. Maiúsculas e
minúsculas são diferenciadas.

| Arquivo | Uso |
|---|---|
| `entrada.csv` | Grafo principal: 8 vértices e 18 arestas |
| `grafo_1.csv` | Grafo pequeno com 4 vértices |
| `acentos.csv` | Rótulos com acentos |
| `arvore.csv` | Grafo que já é uma árvore |
| `ciclo.csv` | Ciclo de 6 vértices: BFS rasa, DFS profunda |
| `completo_k5.csv` | Grafo completo com 5 vértices |
| `desconexo.csv` | Três componentes; Prim e Kruskal geram floresta |
| `estrela.csv` | Centro ligado a todos os outros vértices |
| `dijkstra_armadilha.csv` | Caminho mais barato com mais arestas que o direto |
| `invalido.csv` | Peso inválido, para demonstrar a mensagem de erro |

## Testar pelo menu

Para carregar `entrada.csv`, executar a BFS a partir de `Miguel` e sair:

```sh
printf '1\n7\n\n8\nMiguel\n\n0\n' | ./out/grafo
```
