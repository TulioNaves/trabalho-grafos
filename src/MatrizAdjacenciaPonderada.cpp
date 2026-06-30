#include "TeoriaDosGrafos/MatrizAdjacenciaPonderada.hpp"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <numeric>

namespace TeoriaDosGrafos {

void MatrizAdjacenciaPonderada::carregarDoArquivo(const std::string& caminhoArquivo, bool direcionado) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminhoArquivo);
    }

    arquivo >> numVertices;

    // Trava de segurança: impede o carregamento se a matriz de doubles exceder 4 GB
    const size_t LIMITE_MEMORIA = 4ULL * 1024 * 1024 * 1024;
    size_t numElementos = static_cast<size_t>(numVertices + 1) * (numVertices + 1);
    size_t estimativaBytes = numElementos * sizeof(double);

    if (estimativaBytes > LIMITE_MEMORIA) {
        throw std::runtime_error("Grafo muito grande para Matriz Ponderada (Estimativa: " + 
                                 std::to_string(estimativaBytes / 1024 / 1024) + " MB). Pulando...");
    }

    try {
        matriz.assign(numElementos, INF);
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Falha na alocacao de memoria para a matriz ponderada");
    }

    this->direcionado = direcionado;
    numArestas = 0;
    temPesoNegativo = false;

    int u, v;
    double peso;
    while (arquivo >> u >> v >> peso) {
        if (u > numVertices || v > numVertices || u < 1 || v < 1) continue;
        
        size_t idx = getIndex(u, v);
        if (matriz[idx] == INF) {
            matriz[idx] = peso;
            if (!direcionado) {
                matriz[getIndex(v, u)] = peso;
            }
            numArestas++;
            if (peso < 0) temPesoNegativo = true;
        }
    }

    arquivo.close();
}

std::vector<std::pair<int, double>> MatrizAdjacenciaPonderada::getVizinhosPonderados(int v) const {
    if (v < 1 || v > numVertices) return {};
    std::vector<std::pair<int, double>> vizinhos;
    for (int j = 1; j <= numVertices; ++j) {
        double peso = matriz[getIndex(v, j)];
        if (peso != INF) {
            vizinhos.push_back({j, peso});
        }
    }
    return vizinhos;
}

std::vector<int> MatrizAdjacenciaPonderada::getVizinhos(int v) const {
    if (v < 1 || v > numVertices) return {};
    std::vector<int> vizinhos;
    for (int j = 1; j <= numVertices; ++j) {
        if (matriz[getIndex(v, j)] != INF) {
            vizinhos.push_back(j);
        }
    }
    return vizinhos;
}

int MatrizAdjacenciaPonderada::getGrau(int v) const {
    if (v < 1 || v > numVertices) return 0;
    int grau = 0;
    for (int j = 1; j <= numVertices; ++j) {
        if (matriz[getIndex(v, j)] != INF) grau++;
    }
    return grau;
}

EstatisticasGrafo MatrizAdjacenciaPonderada::calcularEstatisticas() {
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

void MatrizAdjacenciaPonderada::gerarArquivoSaida(const std::string& caminhoArquivo) {
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
