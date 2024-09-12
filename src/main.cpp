#include <iostream>
#include "reader.h" 
#include "guloso.h"  
#include "neighbor.h"

void BuscaLocal(Solucao& solucao, std::vector<std::vector<int>>& s){
    std::vector<int> metodos = {0,1,2};
    bool melhorou = false;

    while(!metodos.empty()){
        int n = rand() % metodos.size();
        switch (metodos[n]){
            case 0:
                //std::cout<< "Swap Movement\n" << std::endl;
                melhorou = bestImprovementSwap(solucao, s);
                break;
            case 1:
                //std::cout<< "Insert Movement\n" << std::endl;
                melhorou = bestImprovementInsert(solucao, s);
                break;
            case 2:
                //std::cout<< "2-opt Movement\n" << std::endl;
                melhorou = bestImprovement2opt(solucao, s);
                break;
        }

        if(melhorou){
            metodos = {0,1,2};
        } else {
            metodos.erase(metodos.begin() + n);
        }
    }

    std::cout << "Ordem após a busca local: ";
    for (const auto& pedido : solucao.pedidos) {
        std::cout << pedido.id << " ";
    }
    std::cout << std::endl;

    std::cout << "Multa após a busca local: " << solucao.multa << std::endl;
}

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

    BuscaLocal(solucao, s);
    
    return 0;
    
}