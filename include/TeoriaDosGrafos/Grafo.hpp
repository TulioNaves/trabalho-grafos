#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <string>
#include <vector>

namespace TeoriaDosGrafos {

struct EstatisticasGrafo {
    int numVertices;
    int numArestas;
    int grauMinimo;
    int grauMaximo;
    float grauMedio;
    float medianaGrau;
};

class Grafo {
public:
    virtual ~Grafo() = default;

    virtual void carregarDoArquivo(const std::string& caminhoArquivo, bool direcionado = false) = 0;
    virtual void gerarArquivoSaida(const std::string& caminhoArquivo) = 0;

    virtual EstatisticasGrafo calcularEstatisticas() = 0;
    
    // Métodos de abstração para os algoritmos
    virtual std::vector<int> getVizinhos(int v) const = 0;
    virtual int getGrau(int v) const = 0;

    int getNumVertices() const { return numVertices; }
    int getNumArestas() const { return numArestas; }
    bool isDirecionado() const { return direcionado; }
    void setDirecionado(bool dir) { direcionado = dir; }

protected:
    int numVertices = 0;
    int numArestas = 0;
    bool direcionado = false;
};

} // namespace TeoriaDosGrafos

#endif // GRAFO_HPP
