# Biblioteca de Teoria dos Grafos (Projeto Unificado P1, P2 & P3)

Esta biblioteca em C++ foi desenvolvida para a disciplina de **Teoria dos Grafos (COS 242)**. O sistema é capaz de manipular grafos direcionados e não-direcionados de larga escala, suportando tanto grafos simples (sem peso) quanto grafos ponderados, com foco em eficiência de memória, controle de complexidade e performance algorítmica.

---

## 🏗️ Decisões de Arquitetura e Implementação

O projeto foi construído seguindo princípios de **Engenharia de Software** para garantir robustez e extensibilidade:

1.  **Polimorfismo e Suporte a Grafos Direcionados (Parte 3):** 
    Estendemos a classe base `Grafo` e suas derivadas (`ListaAdjacencia`, `MatrizAdjacencia`, `ListaAdjacenciaPonderada`, `MatrizAdjacenciaPonderada`) para suportar a flag `direcionado`. No caso de grafos direcionados, as arestas só são inseridas no sentido original ($u \to v$), enquanto os métodos de grau e estatísticas reportam o **grau de saída (out-degree)**.
2.  **Transposição de Grafos:**
    Para algoritmos de caminhos mínimos de múltiplos vértices para um único destino (como o caso de encontrar caminhos *para* o vértice 100 no Estudo de Caso), implementamos o método `gerarGrafoTransposto`. Esse método inverte a direção de todas as arestas do grafo, permitindo usar os algoritmos clássicos de fonte única (Dijkstra e Bellman-Ford) a partir do destino de interesse.
3.  **Algoritmo de Bellman-Ford Otimizado:**
    Implementamos o Bellman-Ford clássico capaz de detectar ciclos de peso negativo e incorporamos as duas melhorias discutidas em aula:
    *   **Early Termination (Encerramento Antecipado):** Interrompe o loop principal caso nenhuma distância sofra relaxamento durante uma iteração completa.
    *   **1ª Otimização de Yen:** Particiona as relaxações de arestas em passagens progressivas (forward, $u < v$) e regressivas (backward, $u > v$) ordenadas pelos IDs dos vértices, agilizando drasticamente a propagação dos caminhos mínimos.
4.  **Travas de Segurança de Memória (RAM):**
    Mantemos a trava de **4 GB** preditiva. Antes de alocar qualquer representação de grafo, o sistema calcula o consumo teórico e aborta o carregamento com um erro caso exceda 4 GB, protegendo o sistema operacional.
5.  **Controle de Complexidade Algorítmica e Segurança:**
    *   O Dijkstra com Vetor ($O(V^2)$) é automaticamente ignorado para grafos com mais de **20.000 vértices**.
    *   O Bellman-Ford Clássico ($O(V \cdot E)$) é pulado para grafos com mais de **10.000 vértices** para evitar travamento da CPU, rodando apenas a versão otimizada.

---

## 🛠️ Classes e Responsabilidades

### Interfaces (Abstração)
- **`Grafo`**: Contrato base para representações de grafos (guarda a flag `direcionado`).
- **`GrafoPonderado`**: Estende `Grafo`, exigindo métodos para recuperação de pesos das arestas.

### Representações (Dados)
- **`ListaAdjacencia` / `ListaAdjacenciaPonderada`**: Armazenamento esparso via `std::vector<std::vector<...>>`. Recomendado para grafos esparsos de grande escala devido ao baixo uso de memória.
- **`MatrizAdjacencia`**: Representação densa de grafos simples usando `std::vector<bool>` (bitset) onde cada aresta ocupa apenas 1 bit.
- **`MatrizAdjacenciaPonderada`**: Matriz densa de doubles simulada em vetor linear com indexação em $O(1)$.

### Algoritmos e Auxiliares
- **`AlgoritmosGrafos`**: Implementa BFS, DFS, Componentes Conexas, Dijkstra (Vetor e Heap), Bellman-Ford (Clássico e Otimizado) e Transposição de Grafos.
- **`MinHeap`**: Min-Heap Indexado customizado de alta performance. Permite a operação "Decrease Key" em $O(\log V)$, crucial para a eficiência do Dijkstra.

---

## 🔄 Fluxo de Dados

### 1. Entrada (Input)
O programa espera arquivos de texto `.txt` estruturados conforme as partes do projeto:
- `data/grafos/`: Grafos simples sem pesos (Número de vértices na linha 1, e pares `U V` nas seguintes).
- `data/grafos_pesos/`: Grafos ponderados (Número de vértices na linha 1, e triplas `U V Peso` nas seguintes). No caso de grafos direcionados, a direção segue de `U` para `V`.

### 2. Saída (Output - CSV)
Todos os relatórios e benchmarks comparativos são gravados na pasta `data/resultados/`:
- `resultados_benchmarking_p1.csv`: Performance de BFS/DFS e consumo de RAM.
- `resultados_benchmarking_p2.csv`: Comparativo do Dijkstra (Vetor vs Heap).
- `resultados_p3_distancias.csv`: Tabela de distâncias dos vértices 10, 20 e 30 para o vértice 100 obtidas por Bellman-Ford e Dijkstra.
- `resultados_p3_benchmarking.csv`: Tempos médios de execução (em segundos) e número de iterações do Bellman-Ford Otimizado vs Clássico vs Dijkstra Heap.
- `caminhos_colaboracao.csv`: Resultados da análise entre o Prof. Edsger Dijkstra e outros pesquisadores na rede de colaboração.

---

## 🚀 Como Executar

O projeto utiliza um **Makefile** para compilação robusta e otimizada (`-O3`).

### Compilação
```bash
make clean
make
```

### Execução (Menu Interativo)
```bash
./bin/grafo_bench
```
O programa exibirá um menu interativo com cinco opções:
1.  **Executar Projeto 1**: BFS, DFS e Componentes Conexas.
2.  **Executar Projeto 2**: Dijkstra Vetor vs Heap e caminhos do vértice 10.
3.  **Executar Análise da Rede de Colaboração**: Análise Edsger Dijkstra na rede de cientistas.
4.  **Executar Projeto 3**: Estudos de Caso e benchmarking do Bellman-Ford (Otimizado/Clássico) vs Dijkstra Heap em grafos direcionados.
5.  **Sair**

---

## 📊 Monitoramento de Recursos
A classe `MonitorMemoria` realiza a leitura de baixo nível do arquivo `/proc/self/status` do Linux para capturar o RSS (Resident Set Size), garantindo medições precisas da quantidade de memória física consumida durante a alocação de grafos gigantescos.
