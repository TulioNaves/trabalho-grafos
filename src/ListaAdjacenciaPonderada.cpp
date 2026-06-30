#include "TeoriaDosGrafos/ListaAdjacenciaPonderada.hpp"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <numeric>

namespace TeoriaDosGrafos {

void ListaAdjacenciaPonderada::carregarDoArquivo(const std::string& caminhoArquivo, bool direcionado) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    arquivo >> numVertices;

    // Trava de segurança: impede o carregamento se o custo base exceder 4 GB
    const size_t LIMITE_MEMORIA = 8ULL * 1024 * 1024 * 1024;
    size_t estimativaBaseBytes = static_cast<size_t>(numVertices + 1) * sizeof(std::vector<std::pair<int, double>>);

    if (estimativaBaseBytes > LIMITE_MEMORIA) {
        throw std::runtime_error("Grafo muito grande para Lista Adjacencia Ponderada. Pulando...");
    }

    this->direcionado = direcionado;
    adj.assign(numVertices + 1, std::vector<std::pair<int, double>>());
    numArestas = 0;
    temPesoNegativo = false;

    int u, v;
    double peso;
    while (arquivo >> u >> v >> peso) {
        if (u > numVertices || v > numVertices || u < 1 || v < 1) continue;
        
        adj[u].push_back({v, peso});
        if (!direcionado) {
            adj[v].push_back({u, peso});
        }
        numArestas++;
        if (peso < 0) temPesoNegativo = true;
    }

    arquivo.close();
}

void ListaAdjacenciaPonderada::inicializar(int n) {
    numVertices = n;
    adj.assign(numVertices + 1, std::vector<std::pair<int, double>>());
    numArestas = 0;
    temPesoNegativo = false;
}

void ListaAdjacenciaPonderada::adicionarAresta(int u, int v, double peso) {
    if (u > numVertices || v > numVertices || u < 1 || v < 1) return;
    adj[u].push_back({v, peso});
    if (!direcionado) {
        adj[v].push_back({u, peso});
    }
    numArestas++;
    if (peso < 0) temPesoNegativo = true;
}

std::vector<std::pair<int, double>> ListaAdjacenciaPonderada::getVizinhosPonderados(int v) const {
    if (v < 1 || v > numVertices) return {};
    return adj[v];
}

const std::vector<std::pair<int, double>>& ListaAdjacenciaPonderada::getVizinhosPonderadosRef(int v) const {
    static const std::vector<std::pair<int, double>> vazio;
    if (v < 1 || v > numVertices) return vazio;
    return adj[v];
}

std::vector<int> ListaAdjacenciaPonderada::getVizinhos(int v) const {
    if (v < 1 || v > numVertices) return {};
    std::vector<int> vizinhos;
    for (const auto& par : adj[v]) {
        vizinhos.push_back(par.first);
    }
    return vizinhos;
}

int ListaAdjacenciaPonderada::getGrau(int v) const {
    if (v < 1 || v > numVertices) return 0;
    return static_cast<int>(adj[v].size());
}

EstatisticasGrafo ListaAdjacenciaPonderada::calcularEstatisticas() {
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

void ListaAdjacenciaPonderada::gerarArquivoSaida(const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) throw std::runtime_error("Falha ao abrir arquivo de saida");

    EstatisticasGrafo stats = calcularEstatisticas();
    arquivo << "Número de vértices: " << stats.numVertices << "\n";
    arquivo << "Número de arestas: " << stats.numArestas << "\n";
    arquivo << "Grau mínimo: " << stats.grauMinimo << "\n";
    arquivo << "Grau máximo: " << stats.grauMaximo << "\n";
    arquivo << "Grau médio: " << stats.grauMedio << "\n";
    arquivo << "Mediana de grau: " << stats.medianaGrau << "\n";
    arquivo << "Possui peso negativo: " << (temPesoNegativo ? "Sim" : "Não") << "\n";
    arquivo.close();
}

} // namespace TeoriaDosGrafos
