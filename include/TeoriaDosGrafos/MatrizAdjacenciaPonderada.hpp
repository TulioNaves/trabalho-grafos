#ifndef MATRIZ_ADJACENCIA_PONDERADA_HPP
#define MATRIZ_ADJACENCIA_PONDERADA_HPP

#include "GrafoPonderado.hpp"
#include <vector>
#include <limits>

namespace TeoriaDosGrafos {

class MatrizAdjacenciaPonderada : public GrafoPonderado {
public:
    void carregarDoArquivo(const std::string& caminhoArquivo, bool direcionado = false) override;
    void gerarArquivoSaida(const std::string& caminhoArquivo) override;
    EstatisticasGrafo calcularEstatisticas() override;

    std::vector<int> getVizinhos(int v) const override;
    int getGrau(int v) const override;

    // Implementação da interface GrafoPonderado
    std::vector<std::pair<int, double>> getVizinhosPonderados(int v) const override;
    bool possuiPesoNegativo() const override { return temPesoNegativo; }

private:
    std::vector<double> matriz;
    bool temPesoNegativo = false;
    const double INF = std::numeric_limits<double>::infinity();

    inline size_t getIndex(int u, int v) const {
        return static_cast<size_t>(u) * (numVertices + 1) + v;
    }
};

} // namespace TeoriaDosGrafos

#endif // MATRIZ_ADJACENCIA_PONDERADA_HPP
