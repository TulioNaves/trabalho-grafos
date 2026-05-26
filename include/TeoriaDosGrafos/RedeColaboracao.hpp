#ifndef REDE_COLABORACAO_HPP
#define REDE_COLABORACAO_HPP

#include "ListaAdjacenciaPonderada.hpp"
#include "MapeadorVertices.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <tuple>

namespace TeoriaDosGrafos {

class RedeColaboracao {
public:
    static std::pair<std::unique_ptr<ListaAdjacenciaPonderada>, MapeadorVertices> 
    carregarDeArquivo(const std::string& caminhoArestas, const std::string& caminhoVertices) {
        MapeadorVertices mapeador;
        
        // 1. Carregar mapeamento ID -> Nome
        std::ifstream arquivoVertices(caminhoVertices);
        if (!arquivoVertices.is_open()) throw std::runtime_error("Erro ao abrir vertices: " + caminhoVertices);
        
        std::string linha;
        int maxId = 0;
        while (std::getline(arquivoVertices, linha)) {
            if (linha.empty()) continue;
            size_t virgula = linha.find(',');
            if (virgula != std::string::npos) {
                int id = std::stoi(linha.substr(0, virgula));
                std::string nome = linha.substr(virgula + 1);
                mapeador.definirMapeamento(id, nome);
                if (id > maxId) maxId = id;
            }
        }
        arquivoVertices.close();

        // 2. Carregar Arestas
        std::ifstream arquivoArestas(caminhoArestas);
        if (!arquivoArestas.is_open()) throw std::runtime_error("Erro ao abrir arestas: " + caminhoArestas);

        int n;
        arquivoArestas >> n; // Lê o número de vértices da primeira linha
        if (n < maxId) n = maxId;

        auto grafo = std::make_unique<ListaAdjacenciaPonderada>();
        grafo->inicializar(n);

        int u, v;
        double peso;
        while (arquivoArestas >> u >> v >> peso) {
            grafo->adicionarAresta(u, v, peso);
        }
        
        return {std::move(grafo), std::move(mapeador)};
    }
};

}

#endif // REDE_COLABORACAO_HPP
