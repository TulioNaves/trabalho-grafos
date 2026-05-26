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
#include "TeoriaDosGrafos/ListaAdjacenciaPonderada.hpp"
#include "TeoriaDosGrafos/MatrizAdjacenciaPonderada.hpp"
#include "TeoriaDosGrafos/MonitorMemoria.hpp"
#include "TeoriaDosGrafos/RedeColaboracao.hpp"
#include "TeoriaDosGrafos/MapeadorVertices.hpp"

using namespace TeoriaDosGrafos;
namespace fs = std::filesystem;

struct ResultadoP1 {
    std::string arquivo;
    std::string representacao;
    int numVertices;
    int numArestas;
    long memoriaKB;
    double tempoBFS;
    double tempoDFS;
};

struct ResultadoP2 {
    std::string arquivo;
    std::string representacao;
    int numVertices;
    int numArestas;
    double tempoDijkstraVetor;
    double tempoDijkstraHeap;
};

std::string formatarCaminho(const std::vector<int>& caminho) {
    if (caminho.empty()) return "Sem caminho";
    std::string s = "";
    for (size_t i = 0; i < caminho.size(); ++i) {
        s += std::to_string(caminho[i]) + (i == caminho.size() - 1 ? "" : " -> ");
    }
    return s;
}

void gerarTabelaCaminhos(const GrafoPonderado& g, const std::string& nomeArquivo, int origem, const std::vector<int>& destinos) {
    auto [distancias, pais] = AlgoritmosGrafos::dijkstraHeap(g, origem);
    
    std::ofstream out("data/resultados/" + nomeArquivo + "_caminhos_10.csv");
    out << "Origem,Destino,Distancia,Caminho\n";

    for (int d : destinos) {
        if (d > g.getNumVertices()) {
            out << origem << "," << d << ",N/A,Vertice inexistente\n";
            continue;
        }
        double dist = distancias[d];
        std::string distStr = (dist == std::numeric_limits<double>::infinity() ? "Inf" : std::to_string(dist));
        auto caminho = AlgoritmosGrafos::recuperarCaminho(origem, d, pais);
        out << origem << "," << d << "," << distStr << ",\"" << formatarCaminho(caminho) << "\"\n";
    }
    out.close();
}

void pipelineProjeto1() {
    std::string pastaGrafos = "data/grafos";
    std::string pastaResultados = "data/resultados";
    std::vector<ResultadoP1> resultados;

    if (!fs::exists(pastaGrafos)) {
        std::cerr << "Pasta 'data/grafos' nao encontrada!" << std::endl;
        return;
    }
    
    std::vector<std::string> arquivos;
    for (const auto& entry : fs::directory_iterator(pastaGrafos)) {
        if (entry.path().extension() == ".txt") arquivos.push_back(entry.path().string());
    }
    std::sort(arquivos.begin(), arquivos.end());

    for (const auto& arquivo : arquivos) {
        for (std::string tipo : {"Lista", "Matriz"}) {
            std::cout << "P1: Processando " << fs::path(arquivo).filename() << " (" << tipo << ")..." << std::endl;
            try {
                std::unique_ptr<Grafo> g;
                long m1 = MonitorMemoria::getUsoMemoriaKB();
                if (tipo == "Lista") g = std::make_unique<ListaAdjacencia>();
                else g = std::make_unique<MatrizAdjacencia>();
                
                g->carregarDoArquivo(arquivo);
                long m2 = MonitorMemoria::getUsoMemoriaKB();
                
                auto stats = g->calcularEstatisticas();
                std::mt19937 gen(42);
                std::uniform_int_distribution<> dis(1, stats.numVertices);
                
                double tBFS = 0, tDFS = 0;
                int k = 100; // Conforme solicitado no enunciado
                for(int i=0; i<k; ++i) {
                    int raiz = dis(gen);
                    auto s = std::chrono::high_resolution_clock::now();
                    AlgoritmosGrafos::bfs(*g, raiz, "/dev/null");
                    auto e = std::chrono::high_resolution_clock::now();
                    tBFS += std::chrono::duration<double, std::milli>(e-s).count();

                    s = std::chrono::high_resolution_clock::now();
                    AlgoritmosGrafos::dfs(*g, raiz, "/dev/null");
                    e = std::chrono::high_resolution_clock::now();
                    tDFS += std::chrono::duration<double, std::milli>(e-s).count();
                }

                resultados.push_back({fs::path(arquivo).filename().string(), tipo, stats.numVertices, stats.numArestas, m2-m1, tBFS/k, tDFS/k});
            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << std::endl;
            }
        }
    }

    std::ofstream csv(pastaResultados + "/resultados_benchmarking_p1.csv");
    csv << "Arquivo,Representacao,Vertices,Arestas,MemoriaKB,TempoBFS_ms,TempoDFS_ms\n";
    for(auto& r : resultados)
        csv << r.arquivo << "," << r.representacao << "," << r.numVertices << "," << r.numArestas << "," << r.memoriaKB << "," << r.tempoBFS << "," << r.tempoDFS << "\n";
    std::cout << "Resultados P1 salvos em data/resultados/resultados_benchmarking_p1.csv" << std::endl;
}

void pipelineProjeto2() {
    std::string pastaGrafos = "data/grafos_pesos";
    std::string pastaResultados = "data/resultados";
    std::vector<ResultadoP2> resultados;

    if (!fs::exists(pastaGrafos)) {
        std::cerr << "Pasta 'data/grafos_pesos' nao encontrada!" << std::endl;
        return;
    }

    std::vector<std::string> arquivos;
    for (const auto& entry : fs::directory_iterator(pastaGrafos)) {
        if (entry.path().extension() == ".txt") arquivos.push_back(entry.path().string());
    }
    std::sort(arquivos.begin(), arquivos.end());

    for (const auto& arquivo : arquivos) {
        for (std::string tipo : {"ListaPonderada", "MatrizPonderada"}) {
            std::cout << "P2: Processando " << fs::path(arquivo).filename() << " (" << tipo << ")..." << std::endl;
            try {
                std::unique_ptr<GrafoPonderado> g;
                if (tipo == "ListaPonderada") g = std::make_unique<ListaAdjacenciaPonderada>();
                else g = std::make_unique<MatrizAdjacenciaPonderada>();
                
                g->carregarDoArquivo(arquivo);
                auto stats = g->calcularEstatisticas();

                // Pergunta 1: Caminhos de 10 para 20, 30, 40, 50, 60 (apenas para a Lista para não repetir)
                if (tipo == "ListaPonderada" && stats.numVertices >= 10) {
                    gerarTabelaCaminhos(*g, fs::path(arquivo).stem().string(), 10, {20, 30, 40, 50, 60});
                }
                
                // Pergunta 2: Benchmark de 100 vértices
                std::mt19937 gen(42);
                std::uniform_int_distribution<> dis(1, stats.numVertices);
                
                double tVetor = 0, tHeap = 0;
                int k = (stats.numVertices > 20000) ? 10 : 100; // Ajuste para não demorar horas em grafos gigantes

                for(int i = 0; i < k; ++i) {
                    int raiz = dis(gen);
                    if (stats.numVertices <= 20000) {
                        auto s = std::chrono::high_resolution_clock::now();
                        AlgoritmosGrafos::dijkstraVetor(*g, raiz);
                        auto e = std::chrono::high_resolution_clock::now();
                        tVetor += std::chrono::duration<double, std::milli>(e-s).count();
                    } else {
                        tVetor = -1;
                    }

                    auto s = std::chrono::high_resolution_clock::now();
                    AlgoritmosGrafos::dijkstraHeap(*g, raiz);
                    auto e = std::chrono::high_resolution_clock::now();
                    tHeap += std::chrono::duration<double, std::milli>(e-s).count();
                }

                resultados.push_back({
                    fs::path(arquivo).filename().string(), 
                    tipo, 
                    stats.numVertices, 
                    stats.numArestas, 
                    (tVetor < 0 ? -1 : tVetor / k), 
                    tHeap / k
                });
            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << std::endl;
            }
        }
    }

    std::ofstream csv(pastaResultados + "/resultados_benchmarking_p2.csv");
    csv << "Arquivo,Representacao,Vertices,Arestas,TempoDijkstraVetor_ms,TempoDijkstraHeap_ms\n";
    for(auto& r : resultados)
        csv << r.arquivo << "," << r.representacao << "," << r.numVertices << "," << r.numArestas << "," << r.tempoDijkstraVetor << "," << r.tempoDijkstraHeap << "\n";
    std::cout << "Resultados P2 salvos em data/resultados/resultados_benchmarking_p2.csv" << std::endl;
}

void pipelineRedeColaboracao() {
    std::cout << "Iniciando analise da Rede de Colaboracao...\n";
    std::string arquivoArestas = "data/rede_colaboracao/rede_colaboracao.txt";
    std::string arquivoVertices = "data/rede_colaboracao/rede_colaboracao_vertices.txt";
    
    if (!fs::exists(arquivoArestas) || !fs::exists(arquivoVertices)) {
        std::cerr << "Arquivos da rede de colaboracao nao encontrados em data/rede_colaboracao/!\n";
        return;
    }

    try {
        auto [grafo, mapeador] = RedeColaboracao::carregarDeArquivo(arquivoArestas, arquivoVertices);
        std::cout << "Rede carregada: " << mapeador.totalVertices() << " pesquisadores.\n";

        std::string origemNome = "Edsger W. Dijkstra";
        if (!mapeador.existe(origemNome)) {
            std::cerr << "Pesquisador '" << origemNome << "' nao encontrado na rede!\n";
            return;
        }

        int idOrigem = mapeador.obterID(origemNome);
        auto [distancias, pais] = AlgoritmosGrafos::dijkstraHeap(*grafo, idOrigem);

        std::vector<std::string> destinos = {
            "Alan M. Turing", "J. B. Kruskal", "Jon M. Kleinberg", "Eva Tardos", "Daniel R. Figueiredo"
        };

        std::ofstream out("data/resultados/caminhos_colaboracao.csv");
        out << "Origem,Destino,Distancia,Caminho\n";

        for (const auto& nome : destinos) {
            if (!mapeador.existe(nome)) {
                out << "\"" << origemNome << "\",\"" << nome << "\",N/A,Nao encontrado na rede\n";
                continue;
            }
            int idDest = mapeador.obterID(nome);
            double dist = distancias[idDest];
            auto caminhoIds = AlgoritmosGrafos::recuperarCaminho(idOrigem, idDest, pais);
            
            std::string caminhoNomes = "";
            for (size_t i = 0; i < caminhoIds.size(); ++i) {
                caminhoNomes += mapeador.obterNome(caminhoIds[i]) + (i == caminhoIds.size()-1 ? "" : " -> ");
            }

            out << "\"" << origemNome << "\",\"" << nome << "\"," 
                << (dist == std::numeric_limits<double>::infinity() ? "Inf" : std::to_string(dist))
                << ",\"" << caminhoNomes << "\"\n";
        }
        out.close();
        std::cout << "Analise da rede concluida! Resultados em data/resultados/caminhos_colaboracao.csv\n";

    } catch (const std::exception& e) {
        std::cerr << "Erro na rede de colaboracao: " << e.what() << std::endl;
    }
}

int main() {
    int opcao;
    std::cout << "=== BIBLIOTECA DE GRAFOS (P1 & P2) ===\n";
    std::cout << "1. Executar Projeto 1 (BFS/DFS/Componentes)\n";
    std::cout << "2. Executar Projeto 2 (Dijkstra Vetor vs Heap + Caminhos 10->X)\n";
    std::cout << "3. Executar Analise da Rede de Colaboracao (Edsger Dijkstra -> Outros)\n";
    std::cout << "4. Sair\n";
    std::cout << "Escolha: ";
    if (!(std::cin >> opcao)) return 0;

    if (!fs::exists("data/resultados")) fs::create_directories("data/resultados");

    switch(opcao) {
        case 1: pipelineProjeto1(); break;
        case 2: pipelineProjeto2(); break;
        case 3: pipelineRedeColaboracao(); break;
        case 4: return 0;
        default: std::cout << "Opcao invalida.\n";
    }

    return 0;
}
