#ifndef GRAFO_PONDERADO_HPP
#define GRAFO_PONDERADO_HPP

#include "Grafo.hpp"
#include <vector>
#include <utility>

namespace TeoriaDosGrafos {

/**
 * Interface para grafos que possuem pesos em suas arestas.
 * Herda de Grafo para manter compatibilidade com algoritmos básicos (BFS/DFS),
 * mas adiciona a capacidade de recuperar os pesos para algoritmos como Dijkstra.
 */
class GrafoPonderado : public Grafo {
public:
    virtual ~GrafoPonderado() = default;

    /**
     * Retorna uma lista de pares (vértice vizinho, peso da aresta).
     */
    virtual std::vector<std::pair<int, double>> getVizinhosPonderados(int v) const = 0;

    /**
     * Verifica se o grafo possui algum peso negativo.
     * Útil para validar se o Dijkstra pode ser executado.
     */
    virtual bool possuiPesoNegativo() const = 0;
};

} // namespace TeoriaDosGrafos

#endif // GRAFO_PONDERADO_HPP
