#include "TeoriaDosGrafos/AlgoritmosGrafos.hpp"
#include <fstream>
#include <queue>
#include <stack>
#include <algorithm>
#include <vector>

namespace TeoriaDosGrafos {

void AlgoritmosGrafos::bfs(const Grafo& g, int verticeInicial, const std::string& arquivoSaida) {
    int numVertices = g.getNumVertices();
    if (verticeInicial < 1 || verticeInicial > numVertices) return;

    std::vector<int> pai(numVertices + 1, 0);
    std::vector<int> nivel(numVertices + 1, -1);
    std::queue<int> fila;

    nivel[verticeInicial] = 0;
    fila.push(verticeInicial);

    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();

        for (int v : g.getVizinhos(u)) {
            if (nivel[v] == -1) {
                nivel[v] = nivel[u] + 1;
                pai[v] = u;
                fila.push(v);
            }
        }
    }

    if (arquivoSaida != "/dev/null") {
        std::ofstream arquivo(arquivoSaida);
        if (arquivo.is_open()) {
            for (int i = 1; i <= numVertices; ++i) {
                arquivo << "Vértice " << i << ": Pai " << pai[i] << ", Nível " << nivel[i] << "\n";
            }
            arquivo.close();
        }
    }
}

void AlgoritmosGrafos::dfs(const Grafo& g, int verticeInicial, const std::string& arquivoSaida) {
    int numVertices = g.getNumVertices();
    if (verticeInicial < 1 || verticeInicial > numVertices) return;

    std::vector<int> pai(numVertices + 1, 0);
    std::vector<int> nivel(numVertices + 1, -1);
    std::stack<int> pilha;

    pilha.push(verticeInicial);
    nivel[verticeInicial] = 0;

    while (!pilha.empty()) {
        int u = pilha.top();
        pilha.pop();

        for (int v : g.getVizinhos(u)) {
            if (nivel[v] == -1) {
                nivel[v] = nivel[u] + 1;
                pai[v] = u;
                pilha.push(v);
            }
        }
    }

    if (arquivoSaida != "/dev/null") {
        std::ofstream arquivo(arquivoSaida);
        if (arquivo.is_open()) {
            for (int i = 1; i <= numVertices; ++i) {
                arquivo << "Vértice " << i << ": Pai " << pai[i] << ", Nível " << nivel[i] << "\n";
            }
            arquivo.close();
        }
    }
}

int AlgoritmosGrafos::calcularDistancia(const Grafo& g, int u, int v) {
    int numVertices = g.getNumVertices();
    if (u < 1 || u > numVertices || v < 1 || v > numVertices) return -1;
    if (u == v) return 0;

    std::vector<int> nivel(numVertices + 1, -1);
    std::queue<int> fila;

    nivel[u] = 0;
    fila.push(u);

    while (!fila.empty()) {
        int curr = fila.front();
        fila.pop();

        if (curr == v) return nivel[v];

        for (int vizinho : g.getVizinhos(curr)) {
            if (nivel[vizinho] == -1) {
                nivel[vizinho] = nivel[curr] + 1;
                fila.push(vizinho);
            }
        }
    }

    return -1;
}

int AlgoritmosGrafos::calcularDiametro(const Grafo& g) {
    int numVertices = g.getNumVertices();
    int diametro = 0;
    for (int i = 1; i <= numVertices; ++i) {
        std::vector<int> nivel(numVertices + 1, -1);
        std::queue<int> fila;
        nivel[i] = 0;
        fila.push(i);

        int maxDistLocal = 0;
        while (!fila.empty()) {
            int u = fila.front();
            fila.pop();
            maxDistLocal = std::max(maxDistLocal, nivel[u]);

            for (int v : g.getVizinhos(u)) {
                if (nivel[v] == -1) {
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
            }
        }
        diametro = std::max(diametro, maxDistLocal);
    }
    return diametro;
}

void AlgoritmosGrafos::encontrarComponentesConexas(const Grafo& g, const std::string& arquivoSaida) {
    int numVertices = g.getNumVertices();
    std::vector<bool> visitado(numVertices + 1, false);
    std::vector<std::vector<int>> componentes;

    for (int i = 1; i <= numVertices; ++i) {
        if (!visitado[i]) {
            std::vector<int> componente;
            std::queue<int> fila;
            
            visitado[i] = true;
            fila.push(i);

            while (!fila.empty()) {
                int u = fila.front();
                fila.pop();
                componente.push_back(u);

                for (int v : g.getVizinhos(u)) {
                    if (!visitado[v]) {
                        visitado[v] = true;
                        fila.push(v);
                    }
                }
            }
            componentes.push_back(componente);
        }
    }

    std::sort(componentes.begin(), componentes.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return a.size() > b.size();
    });

    if (arquivoSaida != "/dev/null") {
        std::ofstream arquivo(arquivoSaida);
        if (arquivo.is_open()) {
            arquivo << "Total de componentes conexas: " << componentes.size() << "\n";
            for (size_t i = 0; i < componentes.size(); ++i) {
                arquivo << "Componente " << i + 1 << " (Tamanho " << componentes[i].size() << "): ";
                for (size_t j = 0; j < componentes[i].size(); ++j) {
                    arquivo << componentes[i][j] << (j == componentes[i].size() - 1 ? "" : ", ");
                }
                arquivo << "\n";
            }
            arquivo.close();
        }
    }
}

} // namespace TeoriaDosGrafos
