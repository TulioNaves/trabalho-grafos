#ifndef LISTA_ADJACENCIA_PONDERADA_HPP
#define LISTA_ADJACENCIA_PONDERADA_HPP

#include "GrafoPonderado.hpp"
#include <vector>

namespace TeoriaDosGrafos {

class ListaAdjacenciaPonderada : public GrafoPonderado {
public:
    void carregarDoArquivo(const std::string& caminhoArquivo) override;
    void gerarArquivoSaida(const std::string& caminhoArquivo) override;
    EstatisticasGrafo calcularEstatisticas() override;

    void inicializar(int n);
    void adicionarAresta(int u, int v, double peso);

    std::vector<int> getVizinhos(int v) const override;
    int getGrau(int v) const override;

    // Implementação da interface GrafoPonderado
    std::vector<std::pair<int, double>> getVizinhosPonderados(int v) const override;
    bool possuiPesoNegativo() const override { return temPesoNegativo; }

private:
    std::vector<std::vector<std::pair<int, double>>> adj;
    bool temPesoNegativo = false;
};

} // namespace TeoriaDosGrafos

#endif // LISTA_ADJACENCIA_PONDERADA_HPP
