# Biblioteca de Teoria dos Grafos

Este projeto consiste em uma biblioteca computacional em C++ para manipulação de grafos não-direcionados, desenvolvida para a disciplina de **Teoria dos Grafos (COS 242 - 2025/2)**. A biblioteca é capaz de representar grafos em memória, realizar análises estatísticas e executar algoritmos fundamentais de busca e conectividade.

## 🚀 Propósito
O objetivo principal é fornecer uma ferramenta modular e eficiente para o estudo de grafos de larga escala, permitindo a comparação de desempenho e consumo de memória entre diferentes representações (Lista vs. Matriz de Adjacência).

---

## 🏗️ Arquitetura e Responsabilidades

O projeto segue os princípios de **SRP (Single Responsibility Principle)** e o padrão **Strategy**:

| Classe | Responsabilidade |
| :--- | :--- |
| **`Grafo`** (Interface) | Define o contrato de dados e metadados comuns a qualquer representação. |
| **`ListaAdjacencia`** | Gerencia o armazenamento esparso usando `std::vector<std::vector<int>>`. Otimizada para economia de memória. |
| **`MatrizAdjacencia`** | Gerencia o armazenamento denso usando um vetor linear de bits (`std::vector<bool>`). Otimizada para acesso O(1). |
| **`AlgoritmosGrafos`** | Classe utilitária com métodos estáticos que implementam a lógica matemática (BFS, DFS, Distância, Diâmetro e Componentes Conexas) de forma desacoplada da representação. |
| **`MonitorMemoria`** | Utilitário para medição do uso de memória real (RSS) via sistema Linux. |

---

## 📂 Formatos de Dados

### Entrada (`data/grafos/`)
Arquivos `.txt` seguindo o formato:
- A primeira linha contém o número total de vértices `N`.
- As linhas subsequentes contêm pares `U V` representando uma aresta entre os vértices `U` e `V`.

### Saída (`data/resultados/`)
1.  **`resultados_benchmarking.csv`**: Tabela consolidada com tempos médios (100 execuções), uso de memória e estatísticas de escala.
2.  **`*_estatisticas.txt`**: Relatório de grau mínimo, máximo, médio e mediana de grau.
3.  **`*_componentes.txt`**: Lista de componentes conexas, seus tamanhos e membros, em ordem decrescente.

---

## 🛠️ Como Executar

O projeto utiliza um **Makefile** para automatizar o build.

### Pré-requisitos
- Compilador `g++` com suporte a C++17.
- Sistema Operacional Linux (para monitoramento de memória residente).

### Configuração Inicial (Dados)
Os arquivos de grafos originais não estão incluídos no repositório devido ao seu tamanho. Antes de executar o projeto, você deve:
1. Criar o diretório `data/grafos/` se ele não existir.
2. Adicionar seus arquivos de grafos (ex: `grafo_1.txt`, `grafo_2.txt`, etc.) dentro desta pasta seguindo o formato descrito na seção "Formatos de Dados".

### Comandos Principais

**1. Compilar e Executar tudo:**
```bash
make run
```
Este comando compila o projeto com otimização `-O3` e inicia o pipeline automático de testes em todos os arquivos da pasta `data/grafos/`.

**2. Apenas Compilar:**
```bash
make
```
Gera o executável em `bin/grafo_bench`.

**3. Limpar Build e Resultados:**
```bash
make clean
```
Remove pastas de objetos e todos os arquivos gerados em `data/resultados/`.

---

## 📊 Pipeline de Benchmark
Ao ser executado, o programa:
1. Carrega cada grafo em ambas as representações.
2. Realiza **100 execuções** de BFS e DFS com raízes aleatórias para garantir médias estáveis.
3. Monitora o consumo de RAM real via `/proc/self/status`.
4. Gera relatórios automáticos sem necessidade de intervenção humana.
