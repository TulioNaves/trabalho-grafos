#include "TeoriaDosGrafos/ListaAdjacencia.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>

namespace TeoriaDosGrafos {

void ListaAdjacencia::carregarDoArquivo(const std::string& caminhoArquivo, bool direcionado) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    arquivo >> numVertices;

    // Trava de segurança: impede o carregamento se o custo base dos vetores exceder 4 GB
    const size_t LIMITE_MEMORIA = 4ULL * 1024 * 1024 * 1024; // 4 GB em bytes
    size_t estimativaBaseBytes = static_cast<size_t>(numVertices + 1) * sizeof(std::vector<int>);

    if (estimativaBaseBytes > LIMITE_MEMORIA) {
        throw std::runtime_error("Grafo muito grande para Lista de Adjacencia (Base excede 4 GB). Pulando...");
    }

    this->direcionado = direcionado;
    adj.assign(numVertices + 1, std::vector<int>());
    numArestas = 0;

    int u, v;
    while (arquivo >> u >> v) {
        if (u > numVertices || v > numVertices || u < 1 || v < 1) {
            continue;
        }
        adj[u].push_back(v);
        if (!direcionado) {
            adj[v].push_back(u);
        }
        numArestas++;
    }

    for (int i = 1; i <= numVertices; ++i) {
        std::sort(adj[i].begin(), adj[i].end());
    }

    arquivo.close();
}

void ListaAdjacencia::gerarArquivoSaida(const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo para escrita: " + caminhoArquivo);
    }

    EstatisticasGrafo stats = calcularEstatisticas();

    arquivo << "Número de vértices: " << stats.numVertices << "\n";
    arquivo << "Número de arestas: " << stats.numArestas << "\n";
    arquivo << "Grau mínimo: " << stats.grauMinimo << "\n";
    arquivo << "Grau máximo: " << stats.grauMaximo << "\n";
    arquivo << "Grau médio: " << stats.grauMedio << "\n";
    arquivo << "Mediana de grau: " << stats.medianaGrau << "\n";

    arquivo.close();
}

EstatisticasGrafo ListaAdjacencia::calcularEstatisticas() {
    EstatisticasGrafo stats;
    stats.numVertices = numVertices;
    stats.numArestas = numArestas;

    if (numVertices == 0) {
        stats.grauMinimo = 0;
        stats.grauMaximo = 0;
        stats.grauMedio = 0;
        stats.medianaGrau = 0;
        return stats;
    }

    std::vector<int> graus;
    for (int i = 1; i <= numVertices; ++i) {
        graus.push_back(adj[i].size());
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

std::vector<int> ListaAdjacencia::getVizinhos(int v) const {
    if (v < 1 || v > numVertices) return {};
    return adj[v];
}

int ListaAdjacencia::getGrau(int v) const {
    if (v < 1 || v > numVertices) return 0;
    return static_cast<int>(adj[v].size());
}

} // namespace TeoriaDosGrafos
