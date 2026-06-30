#ifndef LISTA_ADJACENCIA_HPP
#define LISTA_ADJACENCIA_HPP

#include "Grafo.hpp"
#include <vector>
#include <list>

namespace TeoriaDosGrafos {

class ListaAdjacencia : public Grafo {
public:
    ListaAdjacencia() = default;
    ~ListaAdjacencia() override = default;

    void carregarDoArquivo(const std::string& caminhoArquivo, bool direcionado = false) override;
    void gerarArquivoSaida(const std::string& caminhoArquivo) override;
    EstatisticasGrafo calcularEstatisticas() override;

    std::vector<int> getVizinhos(int v) const override;
    int getGrau(int v) const override;

private:
    std::vector<std::vector<int>> adj;
};

} // namespace TeoriaDosGrafos

#endif // LISTA_ADJACENCIA_HPP
