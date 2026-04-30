#ifndef ALGORITMOS_GRAFOS_HPP
#define ALGORITMOS_GRAFOS_HPP

#include "Grafo.hpp"
#include <string>

namespace TeoriaDosGrafos {

class AlgoritmosGrafos {
public:
    static void bfs(const Grafo& g, int verticeInicial, const std::string& arquivoSaida);
    static void dfs(const Grafo& g, int verticeInicial, const std::string& arquivoSaida);

    static int calcularDistancia(const Grafo& g, int u, int v);
    static int calcularDiametro(const Grafo& g);

    static void encontrarComponentesConexas(const Grafo& g, const std::string& arquivoSaida);
};

} // namespace TeoriaDosGrafos

#endif // ALGORITMOS_GRAFOS_HPP
