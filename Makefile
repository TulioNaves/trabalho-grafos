# Compilador e Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Iinclude

# Diretórios
SRC_DIR = src
APPS_DIR = apps
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Arquivos fonte e objetos
LIBRARY_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
LIBRARY_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIBRARY_SRCS))

MAIN_SRC = $(APPS_DIR)/main.cpp
EXECUTABLE = $(BIN_DIR)/grafo_bench

# Regra padrão
all: $(EXECUTABLE)

# Regra para o executável principal
$(EXECUTABLE): $(LIBRARY_OBJS) $(BUILD_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regra para objetos da biblioteca
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para o objeto da main
$(BUILD_DIR)/main.o: $(APPS_DIR)/main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para rodar o benchmark
run: all
	./$(EXECUTABLE)

# Regra para limpar os arquivos de build
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) data/resultados/*.txt data/resultados/*.csv

.PHONY: all run clean
