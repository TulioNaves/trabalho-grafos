#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <vector>
#include <limits>

namespace TeoriaDosGrafos {

/**
 * Estrutura de Min-Heap Indexado.
 * Permite extrair o mínimo e atualizar valores (decrease key) em O(log N).
 */
class MinHeap {
public:
    MinHeap(int maxVertices);

    void inserirOuAtualizar(int v, double distancia);
    int extrairMinimo();
    bool vazio() const;
    bool contem(int v) const;

private:
    struct No {
        int v;
        double distancia;
    };

    std::vector<No> heap;
    std::vector<int> posicao; // Mapeia o ID do vértice para sua posição no vetor 'heap'
    int tamanho;

    void subir(int idx);
    void descer(int idx);
    void trocar(int i, int j);
};

} // namespace TeoriaDosGrafos

#endif // MIN_HEAP_HPP
