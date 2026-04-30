#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <memory>
#include <filesystem>
#include <algorithm>

#include "TeoriaDosGrafos/AlgoritmosGrafos.hpp"
#include "TeoriaDosGrafos/ListaAdjacencia.hpp"
#include "TeoriaDosGrafos/MatrizAdjacencia.hpp"
#include "TeoriaDosGrafos/MonitorMemoria.hpp"

using namespace TeoriaDosGrafos;
namespace fs = std::filesystem;

struct ResultadoBenchmark {
    std::string arquivo;
    std::string representacao;
    int numVertices;
    int numArestas;
    long memoriaKB;
    double tempoMedioBFS_ms;
    double tempoMedioDFS_ms;
};

void executarBenchmark(const std::string& caminhoArquivo, const std::string& tipo, std::vector<ResultadoBenchmark>& resultados) {
    std::cout << "Processando " << caminhoArquivo << " com " << tipo << "..." << std::endl;
    
    std::unique_ptr<Grafo> grafo;
    long memoriaAntes = MonitorMemoria::getUsoMemoriaKB();
    
    try {
        if (tipo == "Lista") {
            grafo = std::make_unique<ListaAdjacencia>();
        } else {
            grafo = std::make_unique<MatrizAdjacencia>();
        }
        
        grafo->carregarDoArquivo(caminhoArquivo);
        
        long memoriaDepois = MonitorMemoria::getUsoMemoriaKB();
        long memoriaGasta = memoriaDepois - memoriaAntes;

        EstatisticasGrafo stats = grafo->calcularEstatisticas();
        
        // Bateria de testes BFS/DFS (100 execuções)
        const int numIteracoes = 100;
        std::mt19937 gen(42); // Seed fixa para reprodutibilidade
        std::uniform_int_distribution<> dis(1, stats.numVertices);
        
        double totalBFS = 0;
        double totalDFS = 0;

        for (int i = 0; i < numIteracoes; ++i) {
            int raiz = dis(gen);
            
            auto s1 = std::chrono::high_resolution_clock::now();
            AlgoritmosGrafos::bfs(*grafo, raiz, "/dev/null"); 
            auto e1 = std::chrono::high_resolution_clock::now();
            totalBFS += std::chrono::duration<double, std::milli>(e1 - s1).count();

            auto s2 = std::chrono::high_resolution_clock::now();
            AlgoritmosGrafos::dfs(*grafo, raiz, "/dev/null");
            auto e2 = std::chrono::high_resolution_clock::now();
            totalDFS += std::chrono::duration<double, std::milli>(e2 - s2).count();
        }

        resultados.push_back({
            fs::path(caminhoArquivo).filename().string(),
            tipo,
            stats.numVertices,
            stats.numArestas,
            memoriaGasta,
            totalBFS / numIteracoes,
            totalDFS / numIteracoes
        });

        // Gerar arquivos de saída específicos
        std::string baseNome = "data/resultados/" + fs::path(caminhoArquivo).stem().string() + "_" + tipo;
        grafo->gerarArquivoSaida(baseNome + "_estatisticas.txt");
        AlgoritmosGrafos::encontrarComponentesConexas(*grafo, baseNome + "_componentes.txt");

    } catch (const std::exception& e) {
        std::cerr << "Erro ao processar " << tipo << ": " << e.what() << std::endl;
        resultados.push_back({
            fs::path(caminhoArquivo).filename().string(),
            tipo,
            -1, -1, 0, 0, 0
        });
    }
}

int main() {
    std::string pastaGrafos = "data/grafos";
    std::string pastaResultados = "data/resultados";
    std::vector<ResultadoBenchmark> resultados;

    if (!fs::exists(pastaGrafos)) {
        std::cerr << "Pasta 'data/grafos' nao encontrada!" << std::endl;
        return 1;
    }
    
    if (!fs::exists(pastaResultados)) {
        fs::create_directories(pastaResultados);
    }

    std::vector<std::string> arquivos;
    for (const auto& entry : fs::directory_iterator(pastaGrafos)) {
        if (entry.path().extension() == ".txt") {
            arquivos.push_back(entry.path().string());
        }
    }
    std::sort(arquivos.begin(), arquivos.end());

    for (const auto& arquivo : arquivos) {
        executarBenchmark(arquivo, "Lista", resultados);
        executarBenchmark(arquivo, "Matriz", resultados);
    }

    std::ofstream csv(pastaResultados + "/resultados_benchmarking.csv");
    csv << "Arquivo,Representacao,Vertices,Arestas,Memoria_KB,Tempo_Medio_BFS_ms,Tempo_Medio_DFS_ms\n";
    for (const auto& r : resultados) {
        csv << r.arquivo << ","
            << r.representacao << ","
            << r.numVertices << ","
            << r.numArestas << ","
            << r.memoriaKB << ","
            << std::fixed << std::setprecision(4) << r.tempoMedioBFS_ms << ","
            << r.tempoMedioDFS_ms << "\n";
    }
    csv.close();

    std::cout << "\nBenchmark concluido! Resultados salvos em " << pastaResultados << "/resultados_benchmarking.csv" << std::endl;

    return 0;
}
