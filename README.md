# Biblioteca de Teoria dos Grafos (Projeto Unificado P1 & P2)

Esta biblioteca em C++ foi desenvolvida para a disciplina de **Teoria dos Grafos (COS 242)**. O sistema é capaz de manipular grafos não-direcionados de larga escala, suportando tanto grafos simples (sem peso) quanto grafos ponderados, com foco em eficiência de memória e performance algorítmica.

---

## 🏗️ Decisões de Arquitetura e Implementação

O projeto foi construído seguindo princípios de **Engenharia de Software** para garantir robustez:

1.  **Polimorfismo e Princípio Aberto-Fechado (OCP):** 
    Para suportar a Parte 2 sem comprometer a performance da Parte 1, estendemos a interface `Grafo` para `GrafoPonderado`. Isso permitiu manter a `MatrizAdjacencia` original (que usa apenas 1 bit por aresta) intacta, enquanto criamos novas classes para os pesos (`double`).
2.  **Travas de Segurança de Memória (RAM):**
    Implementamos travas preditivas de **4 GB**. Antes de carregar qualquer grafo, o sistema calcula o custo teórico de alocação. Se exceder 4 GB, o carregamento é abortado com uma mensagem informativa, protegendo o Sistema Operacional de travamentos.
3.  **Controle de Complexidade Algorítmica:**
    O algoritmo de Dijkstra com Vetor ($O(V^2)$) é automaticamente ignorado para grafos com mais de **20.000 vértices**, onde o tempo de execução se tornaria inviável (dias/semanas). Nesses casos, apenas o Dijkstra com Heap ($O(E \log V)$) é executado.

---

## 🛠️ Classes e Responsabilidades

### Interfaces (Abstração)
- **`Grafo`**: Contrato base para grafos simples.
- **`GrafoPonderado`**: Estende `Grafo`, exigindo métodos para recuperação de pesos das arestas.

### Representações (Dados)
- **`ListaAdjacencia` / `ListaAdjacenciaPonderada`**: Armazenamento esparso via `std::vector<std::vector<...>>`. Ideal para economizar RAM.
- **`MatrizAdjacencia`**: Representação densa via `std::vector<bool>` (bitset). Otimizada para acesso $O(1)$ usando apenas 1 bit por aresta.
- **`MatrizAdjacenciaPonderada`**: Matriz de `double` simulada em vetor linear para máxima performance de cache.

### Algoritmos e Auxiliares
- **`AlgoritmosGrafos`**: Implementa BFS, DFS, Componentes Conexas e as duas versões do Dijkstra.
- **`MinHeap`**: Min-Heap Indexado customizado. Permite o "Decrease Key" em $O(\log V)$, essencial para a eficiência do Dijkstra.
- **`RedeColaboracao` / `MapeadorVertices`**: Utilitários para converter nomes de pesquisadores (strings) em IDs numéricos e vice-versa.

---

## 🔄 Fluxo de Dados

### 1. Entrada (Input)
O programa espera arquivos `.txt` organizados em:
- `data/grafos/`: Grafos simples (N, depois U V).
- `data/grafos_pesos/`: Grafos ponderados (N, depois U V Peso).
- `data/rede_colaboracao/`: Arquivos da rede de pesquisadores (`rede_colaboracao.txt` e `rede_colaboracao_vertices.txt`).

### 2. Saída (Output - CSV)
Todos os resultados de benchmark e caminhos mínimos são salvos em **CSV** para facilitar a análise:
- `resultados_benchmarking_p1.csv`: Performance de BFS/DFS e memória da P1.
- `resultados_benchmarking_p2.csv`: Comparativo Dijkstra Vetor vs Heap da P2.
- `*_caminhos_10.csv`: Tabela de caminhos mínimos do vértice 10 para 20, 30, 40, 50, 60.
- `caminhos_colaboracao.csv`: Resultados da análise entre o Prof. Edsger Dijkstra e outros pesquisadores.

---

## 🚀 Como Executar

O projeto utiliza um **Makefile** para facilitar o build.

### Compilação
```bash
make clean
make
```

### Execução (Menu Interativo)
```bash
./bin/grafo_bench
```
O programa exibirá um menu com três opções principais:
1.  **Projeto 1**: Pipeline completo de busca e componentes.
2.  **Projeto 2**: Pipeline de Dijkstra e caminhos mínimos em grafos ponderados.
3.  **Rede de Colaboração**: Análise específica da rede de pesquisadores.

---

## 📊 Monitoramento de Recursos
A classe `MonitorMemoria` captura o consumo real de RAM (VmRSS) através do sistema de arquivos `/proc` do Linux, permitindo auditoria precisa do uso de hardware durante os testes de carga.
