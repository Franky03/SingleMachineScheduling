#include <iostream>
#include "reader.h" 
#include "guloso.h"  
#include "neighbor.h"

int main(){
    int num_pedidos;
    vector<vector<int>> s;
    Solucao solucao;

    readInstance("../data/input.txt", num_pedidos, solucao.pedidos, s);
    std::cout << "Ordem inicial dos pedidos: ";
    for (const auto& pedido : solucao.pedidos) {
        std::cout << pedido.id << " ";
    }
    std::cout << std::endl;

    solucao.calcularMulta(s);
    std::cout << "Multa antes do algoritmo guloso: " << solucao.multa << std::endl;


    solucao = *gulosao(&solucao, s);

    std::cout << "Ordem após o algoritmo guloso: ";
    for (const auto& pedido : solucao.pedidos) {
        std::cout << pedido.id << " ";
    }
    std::cout << std::endl;

    std::cout << "Multa após o algoritmo guloso: " << solucao.multa << std::endl;

    while(bestImprovementSwap(solucao, s)){
        std::cout << "Multa após a vizinhança: " << solucao.multa << std::endl;
    }

    std::cout << "Ordem após a vizinhança: ";
    for (const auto& pedido : solucao.pedidos) {
        std::cout << pedido.id << " ";
    }
    std::cout << std::endl;
    
    return 0;
    
}