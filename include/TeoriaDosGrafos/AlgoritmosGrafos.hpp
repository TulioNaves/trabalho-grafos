#ifndef ALGORITMOS_GRAFOS_HPP
#define ALGORITMOS_GRAFOS_HPP

#include "Grafo.hpp"
#include "GrafoPonderado.hpp"
#include <string>
#include <vector>

namespace TeoriaDosGrafos {

class AlgoritmosGrafos {
public:
    static void bfs(const Grafo& g, int verticeInicial, const std::string& arquivoSaida);
    static void dfs(const Grafo& g, int verticeInicial, const std::string& arquivoSaida);
    static int calcularDistancia(const Grafo& g, int u, int v);
    static int calcularDiametro(const Grafo& g);
    static void encontrarComponentesConexas(const Grafo& g, const std::string& arquivoSaida);

    // Projeto 2: Algoritmos de Caminho Mínimo
    // Retorna um par: {vetor de distâncias, vetor de pais}
    static std::pair<std::vector<double>, std::vector<int>> dijkstraVetor(const GrafoPonderado& g, int origem);
    static std::pair<std::vector<double>, std::vector<int>> dijkstraHeap(const GrafoPonderado& g, int origem);

    // Utilitário para reconstruir o caminho a partir do vetor de pais
    static std::vector<int> recuperarCaminho(int origem, int destino, const std::vector<int>& pais);
};

} // namespace TeoriaDosGrafos

#endif // ALGORITMOS_GRAFOS_HPP
