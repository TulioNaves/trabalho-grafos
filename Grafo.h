#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>

class Grafo {
private:
    int nVertices;
    int nArestas;
    bool usaMatriz;

    // Representações: Matriz (V^2) e Lista (V + E)
    std::vector<std::vector<bool>> matrizAdj;
    std::vector<std::vector<int>> listaAdj;

public:
    Grafo(bool matriz); // Construtor define a representação
    void lerArquivo(const std::string& caminho);
    void gerarRelatorio(const std::string& caminhoSaida);
    void BFS(int inicio, const std::string& arquivoSaida);
    // Getters para as estatísticas
    int getGrauMin();
    int getGrauMax();
    float getGrauMedio();
    float getMedianaGrau();
};

#endif