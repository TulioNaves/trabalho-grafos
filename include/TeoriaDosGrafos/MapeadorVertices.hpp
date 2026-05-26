#ifndef MAPEADOR_VERTICES_HPP
#define MAPEADOR_VERTICES_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace TeoriaDosGrafos {

/**
 * Classe utilitária para mapear nomes de vértices (strings) para IDs numéricos (inteiros).
 * Essencial para lidar com redes onde os vértices são identificados por nomes ou IDs não sequenciais.
 */
class MapeadorVertices {
public:
    MapeadorVertices() : proximoID(1) {}

    /**
     * Obtém o ID numérico associado a um nome. 
     * Se o nome não existir, ele é adicionado e um novo ID é gerado.
     */
    int obterID(const std::string& nome) {
        auto it = nomeParaID.find(nome);
        if (it != nomeParaID.end()) {
            return it->second;
        }

        int id = proximoID++;
        nomeParaID[nome] = id;
        idParaNome[id] = nome;
        return id;
    }

    /**
     * Obtém o nome original associado a um ID numérico.
     */
    std::string obterNome(int id) const {
        auto it = idParaNome.find(id);
        if (it != idParaNome.end()) {
            return it->second;
        }
        return "";
    }

    int totalVertices() const {
        return static_cast<int>(nomeParaID.size());
    }

    bool existe(const std::string& nome) const {
        return nomeParaID.find(nome) != nomeParaID.end();
    }

    void definirMapeamento(int id, const std::string& nome) {
        nomeParaID[nome] = id;
        idParaNome[id] = nome;
        if (id >= proximoID) proximoID = id + 1;
    }

private:
    std::unordered_map<std::string, int> nomeParaID;
    std::unordered_map<int, std::string> idParaNome;
    int proximoID;
};

} // namespace TeoriaDosGrafos

#endif // MAPEADOR_VERTICES_HPP
