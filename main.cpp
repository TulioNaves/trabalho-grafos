#include "Grafo.h"
#include <iostream>
#include <chrono>
#include <string>

int main() {
    int opcaoEstrutura;
    std::string arquivoGrafo;
    int verticeInicial;

    std::cout << "=== Teste de Desempenho de Grafos ===\n";
    std::cout << "Arquivo de entrada (ex: grafo_teste.txt): ";
    std::cin >> arquivoGrafo;

    std::cout << "\nRepresentacao na Memoria:\n";
    std::cout << "1 - Lista de Adjacencia\n";
    std::cout << "2 - Matriz de Adjacencia\n";
    std::cout << "Opcao: ";
    std::cin >> opcaoEstrutura;

    bool usaMatriz = (opcaoEstrutura == 2);
    Grafo g(usaMatriz);

    std::cout << "\n[1/3] Lendo arquivo...\n";
    auto start_leitura = std::chrono::high_resolution_clock::now();
    g.lerArquivo(arquivoGrafo);
    auto end_leitura = std::chrono::high_resolution_clock::now();
    std::cout << "-> Tempo de Leitura: "
              << std::chrono::duration<double, std::milli>(end_leitura - start_leitura).count() << " ms\n";

    std::cout << "\nVertice inicial para buscas: ";
    std::cin >> verticeInicial;

    std::cout << "\n[2/3] Executando BFS...\n";
    auto start_bfs = std::chrono::high_resolution_clock::now();
    g.BFS(verticeInicial, "bfs_saida.txt");
    auto end_bfs = std::chrono::high_resolution_clock::now();
    std::cout << "-> Tempo da BFS: "
              << std::chrono::duration<double, std::milli>(end_bfs - start_bfs).count() << " ms\n";

    std::cout << "\n[3/3] Gerando estatisticas gerais...\n";
    g.gerarRelatorio("saida.txt");

    std::cout << "\nProcesso concluido. Verifique os arquivos txt gerados.\n";

    return 0;
}