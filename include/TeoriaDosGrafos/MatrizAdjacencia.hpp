#ifndef MATRIZ_ADJACENCIA_HPP
#define MATRIZ_ADJACENCIA_HPP

#include "Grafo.hpp"
#include <vector>

namespace TeoriaDosGrafos {

class MatrizAdjacencia : public Grafo {
public:
    MatrizAdjacencia() = default;
    ~MatrizAdjacencia() override = default;

    void carregarDoArquivo(const std::string& caminhoArquivo) override;
    void gerarArquivoSaida(const std::string& caminhoArquivo) override;
    EstatisticasGrafo calcularEstatisticas() override;

    std::vector<int> getVizinhos(int v) const override;
    int getGrau(int v) const override;

private:
    std::vector<bool> matriz;
    inline size_t getIndex(int u, int v) const {
        return static_cast<size_t>(u) * (numVertices + 1) + v;
    }
};

} // namespace TeoriaDosGrafos

#endif // MATRIZ_ADJACENCIA_HPP
