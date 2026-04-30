#include "Grafo.h"     // CRUCIAL: Sem isso, ele não reconhece nVertices, matrizAdj, etc.
#include <vector>      // Para usar o std::vector
#include <fstream>     // Para usar o std::ofstream (resolve o erro de "incomplete type")
#include <algorithm>   // Para o std::sort
#include <iomanip>     // Para o std::setprecision
#include <iostream>    // Necessário para std::cerr e std::endl
#include <queue>

// 1. Construtor: Inicializa os valores básicos
Grafo::Grafo(bool matriz) : nVertices(0), nArestas(0), usaMatriz(matriz) {}

void Grafo::BFS(int origem, const std::string& arquivoSaida) {
    // Vetores para rastrear o estado de cada vértice
    std::vector<bool> visitado(nVertices + 1, false);
    std::vector<int> pai(nVertices + 1, 0); // 0 indica que não tem pai (raiz)
    std::vector<int> nivel(nVertices + 1, -1); // -1 indica não alcançado
    std::queue<int> fila;

    // Inicializa o vértice de origem
    visitado[origem] = true;
    nivel[origem] = 0;
    fila.push(origem);

    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();

        if (usaMatriz) {
            for (int v = 1; v <= nVertices; ++v) {
                if (matrizAdj[u][v] && !visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
            }
        } else {
            for (int v : listaAdj[u]) {
                if (!visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
            }
        }
    }

    // Grava o resultado no arquivo
    std::ofstream out(arquivoSaida);
    out << "Vertice,Pai,Nivel\n";
    for (int i = 1; i <= nVertices; ++i) {
        out << i << "," << pai[i] << "," << nivel[i] << "\n";
    }
    out.close();
}


// 2. Função de Leitura: O "coração" para carregar o grafo
void Grafo::lerArquivo(const std::string& caminho) {
    std::ifstream file(caminho);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminho << std::endl;
        return;
    }

    file >> nVertices; // Lê o número de vértices (primeira linha)

    if (usaMatriz) {
        matrizAdj.assign(nVertices + 1, std::vector<bool>(nVertices + 1, false));
    } else {
        listaAdj.resize(nVertices + 1);
    }

    int u, v;
    while (file >> u >> v) {
        if (usaMatriz) {
            matrizAdj[u][v] = matrizAdj[v][u] = true;
        } else {
            listaAdj[u].push_back(v);
            listaAdj[v].push_back(u);
        }
        nArestas++;
    }
    file.close();
}

void Grafo::gerarRelatorio(const std::string& caminhoSaida) {
    std::vector<int> graus(nVertices);
    int grauMin = 1e9, grauMax = 0;
    long long somaGraus = 0;

    for (int i = 1; i <= nVertices; ++i) {
        int grauAtual = 0;
        if (usaMatriz) {
            for (int j = 1; j <= nVertices; ++j) {
                if (matrizAdj[i][j]) grauAtual++;
            }
        } else {
            grauAtual = listaAdj[i].size();
        }

        graus[i - 1] = grauAtual;
        if (grauAtual < grauMin) grauMin = grauAtual;
        if (grauAtual > grauMax) grauMax = grauAtual;
        somaGraus += grauAtual;
    }

    // Cálculo da Mediana
    std::sort(graus.begin(), graus.end());
    float mediana;
    if (nVertices % 2 == 0) {
        mediana = (graus[nVertices / 2 - 1] + graus[nVertices / 2]) / 2.0f;
    } else {
        mediana = graus[nVertices / 2];
    }

    // Só calcula se houver vértices para não dar erro 139
    float grauMedio = (nVertices > 0) ? (float)somaGraus / nVertices : 0;

    // Escrita no arquivo (Exigência do Professor)
    std::ofstream out(caminhoSaida);
    out << "n_vertices: " << nVertices << "\n";
    out << "n_arestas: " << nArestas << "\n";
    out << "grau_min: " << grauMin << "\n";
    out << "grau_max: " << grauMax << "\n";
    out << "grau_medio: " << std::fixed << std::setprecision(2) << grauMedio << "\n";
    out << "mediana_grau: " << mediana << "\n";
    out.close();
}