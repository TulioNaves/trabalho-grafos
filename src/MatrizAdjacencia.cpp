#include "TeoriaDosGrafos/MatrizAdjacencia.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>

namespace TeoriaDosGrafos {

void MatrizAdjacencia::carregarDoArquivo(const std::string& caminhoArquivo, bool direcionado) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    arquivo >> numVertices;
    
    // Trava de segurança: impede o carregamento se a estimativa de memória exceder 4 GB
    const size_t LIMITE_MEMORIA = 8ULL * 1024 * 1024 * 1024; // 8 GB em bytes
    size_t estimativaBytes = (static_cast<size_t>(numVertices + 1) * (numVertices + 1)) / 8;

    if (estimativaBytes > LIMITE_MEMORIA) {
        throw std::runtime_error("Grafo muito grande para Matriz de Adjacencia (Estimativa: " + 
                                 std::to_string(estimativaBytes / 1024 / 1024) + " MB). Pulando...");
    }

    try {
        matriz.assign(static_cast<size_t>(numVertices + 1) * (numVertices + 1), false);
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Falha na alocacao de memoria para a matriz");
    }

    this->direcionado = direcionado;
    numArestas = 0;

    int u, v;
    while (arquivo >> u >> v) {
        if (u > numVertices || v > numVertices || u < 1 || v < 1) continue;
        
        size_t idx1 = getIndex(u, v);
        if (!matriz[idx1]) {
            matriz[idx1] = true;
            if (!direcionado) {
                matriz[getIndex(v, u)] = true;
            }
            numArestas++;
        }
    }

    arquivo.close();
}

void MatrizAdjacencia::gerarArquivoSaida(const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) throw std::runtime_error("Falha ao abrir arquivo de saida");

    EstatisticasGrafo stats = calcularEstatisticas();
    arquivo << "Número de vértices: " << stats.numVertices << "\n";
    arquivo << "Número de arestas: " << stats.numArestas << "\n";
    arquivo << "Grau mínimo: " << stats.grauMinimo << "\n";
    arquivo << "Grau máximo: " << stats.grauMaximo << "\n";
    arquivo << "Grau médio: " << stats.grauMedio << "\n";
    arquivo << "Mediana de grau: " << stats.medianaGrau << "\n";
    arquivo.close();
}

EstatisticasGrafo MatrizAdjacencia::calcularEstatisticas() {
    EstatisticasGrafo stats;
    stats.numVertices = numVertices;
    stats.numArestas = numArestas;

    if (numVertices == 0) return {0, 0, 0, 0, 0, 0};

    std::vector<int> graus(numVertices);
    for (int i = 1; i <= numVertices; ++i) {
        graus[i-1] = getGrau(i);
    }

    std::sort(graus.begin(), graus.end());
    stats.grauMinimo = graus.front();
    stats.grauMaximo = graus.back();
    long long somaGraus = std::accumulate(graus.begin(), graus.end(), 0LL);
    stats.grauMedio = static_cast<float>(somaGraus) / numVertices;

    if (numVertices % 2 == 0) {
        stats.medianaGrau = (graus[numVertices / 2 - 1] + graus[numVertices / 2]) / 2.0f;
    } else {
        stats.medianaGrau = static_cast<float>(graus[numVertices / 2]);
    }

    return stats;
}

std::vector<int> MatrizAdjacencia::getVizinhos(int v) const {
    if (v < 1 || v > numVertices) return {};
    std::vector<int> vizinhos;
    for (int j = 1; j <= numVertices; ++j) {
        if (matriz[getIndex(v, j)]) {
            vizinhos.push_back(j);
        }
    }
    return vizinhos;
}

int MatrizAdjacencia::getGrau(int v) const {
    if (v < 1 || v > numVertices) return 0;
    int grau = 0;
    for (int j = 1; j <= numVertices; ++j) {
        if (matriz[getIndex(v, j)]) grau++;
    }
    return grau;
}

} // namespace TeoriaDosGrafos
