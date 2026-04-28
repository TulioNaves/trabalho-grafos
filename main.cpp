#include "Grafo.h"
#include <iostream>

int main() {
    Grafo g(false);

    std::cout << "--- Lendo o arquivo de grafo ---" << std::endl;
    g.lerArquivo("grafo_teste.txt");

    // Adicione esta verificação simples:
    // Se nVertices for 0, significa que a leitura falhou.
    // (Você pode criar um getter para nVertices no Grafo.h se precisar)

    std::cout << "--- Gerando estatisticas ---" << std::endl;
    g.gerarRelatorio("saida.txt");

    return 0;
}