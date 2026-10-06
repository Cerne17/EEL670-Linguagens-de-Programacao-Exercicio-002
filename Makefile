# Autor: Miguel Badany Cerne
# DRE: 123370433
# Arquivo: Makefile
# Título: Compilação do programa
# Descrição: Compila src/, procura cabeçalhos em library/ e gera a saída em out/.

CXX = g++
CPPFLAGS = -Ilibrary
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

# qualquer .cpp em src/ vira um .o em out/
FONTES = $(wildcard src/*.cpp)
OBJETOS = $(patsubst src/%.cpp,out/%.o,$(FONTES))
CABECALHOS = $(wildcard library/*.hpp library/*.tpp)
EXECUTAVEL = out/grafo

# os testes têm main próprio: reaproveitam os objetos de src/, menos main.o
TESTES_FONTES = $(wildcard tests/*.cpp)
TESTES_OBJETOS = $(patsubst tests/%.cpp,out/tests/%.o,$(TESTES_FONTES))
TESTES_CABECALHOS = $(wildcard tests/*.hpp)
TESTES_EXECUTAVEL = out/testes
OBJETOS_SEM_MAIN = $(filter-out out/main.o,$(OBJETOS))

.PHONY: all run clean grafo test

all: $(EXECUTAVEL)

grafo: $(EXECUTAVEL)

$(EXECUTAVEL): $(OBJETOS)
	$(CXX) $(CXXFLAGS) $^ -o $@

out/%.o: src/%.cpp $(CABECALHOS) Makefile
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TESTES_EXECUTAVEL): $(TESTES_OBJETOS) $(OBJETOS_SEM_MAIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

out/tests/%.o: tests/%.cpp $(TESTES_CABECALHOS) $(CABECALHOS) Makefile
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -Itests $(CXXFLAGS) -c $< -o $@

test: $(TESTES_EXECUTAVEL)
	./$(TESTES_EXECUTAVEL)

run: $(EXECUTAVEL)
	./$(EXECUTAVEL)

clean:
	rm -f $(OBJETOS) $(EXECUTAVEL) $(TESTES_OBJETOS) $(TESTES_EXECUTAVEL)
