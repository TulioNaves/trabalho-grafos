#include "TeoriaDosGrafos/MinHeap.hpp"
#include <stdexcept>

namespace TeoriaDosGrafos {

MinHeap::MinHeap(int maxVertices) : tamanho(0) {
    posicao.assign(maxVertices + 1, -1);
}

void MinHeap::inserirOuAtualizar(int v, double distancia) {
    if (contem(v)) {
        int idx = posicao[v];
        if (distancia < heap[idx].distancia) {
            heap[idx].distancia = distancia;
            subir(idx);
        }
    } else {
        posicao[v] = tamanho;
        heap.push_back({v, distancia});
        subir(tamanho);
        tamanho++;
    }
}

int MinHeap::extrairMinimo() {
    if (vazio()) throw std::runtime_error("Heap vazio");

    int minV = heap[0].v;
    posicao[minV] = -1;

    tamanho--;
    if (tamanho > 0) {
        heap[0] = heap[tamanho];
        posicao[heap[0].v] = 0;
        heap.pop_back();
        descer(0);
    } else {
        heap.pop_back();
    }

    return minV;
}

bool MinHeap::vazio() const {
    return tamanho == 0;
}

bool MinHeap::contem(int v) const {
    if (v < 0 || v >= (int)posicao.size()) return false;
    return posicao[v] != -1;
}

void MinHeap::subir(int idx) {
    while (idx > 0) {
        int pai = (idx - 1) / 2;
        if (heap[idx].distancia < heap[pai].distancia) {
            trocar(idx, pai);
            idx = pai;
        } else {
            break;
        }
    }
}

void MinHeap::descer(int idx) {
    while (true) {
        int menor = idx;
        int esq = 2 * idx + 1;
        int dir = 2 * idx + 2;

        if (esq < tamanho && heap[esq].distancia < heap[menor].distancia)
            menor = esq;
        if (dir < tamanho && heap[dir].distancia < heap[menor].distancia)
            menor = dir;

        if (menor != idx) {
            trocar(idx, menor);
            idx = menor;
        } else {
            break;
        }
    }
}

void MinHeap::trocar(int i, int j) {
    No temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;

    posicao[heap[i].v] = i;
    posicao[heap[j].v] = j;
}

} // namespace TeoriaDosGrafos
