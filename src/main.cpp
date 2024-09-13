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

    /* std::cout << "Ordem após a busca local: ";
    for (const auto& pedido : solucao.pedidos) {
        std::cout << pedido.id << " ";
    }
    std::cout << std::endl;

    std::cout << "Multa após a busca local: " << solucao.multa << std::endl; */
}

void DoubleBridge(Solucao &solucao){
    int n = solucao.pedidos.size();

    int pos1 = 1 + rand() % (n / 4);
    int pos2 = pos1 + 1 + rand() % (n / 4);
    int pos3 = pos2 + 1 + rand() % (n / 4);
    
    std::vector<Pedido> novoVetor;
    novoVetor.insert(novoVetor.end(), solucao.pedidos.begin(), solucao.pedidos.begin() + pos1); // A
    novoVetor.insert(novoVetor.end(), solucao.pedidos.begin() + pos3, solucao.pedidos.end()); // D
    novoVetor.insert(novoVetor.end(), solucao.pedidos.begin() + pos2, solucao.pedidos.begin() + pos3); // C
    novoVetor.insert(novoVetor.end(), solucao.pedidos.begin() + pos1, solucao.pedidos.begin() + pos2); // B

    solucao.pedidos = novoVetor;
}

void Perturbar(Solucao &solucao){
    // exemplo: fazer um movimento aleatório de troca de dois pedidos
    int i = rand() % solucao.pedidos.size();
    int j = rand() % solucao.pedidos.size();

    // garantir que i e j não sejam iguais
    while (i == j) {
        j = rand() % solucao.pedidos.size();
    }

    std::swap(solucao.pedidos[i], solucao.pedidos[j]);
}

void ILS(Solucao &solucao, std::vector<std::vector<int>>& s, int maxIter, int maxIterILS){
    Solucao melhorSolucao;
    for(int i=0; i < maxIter; ++i){
        Solucao novaSolucao = *Construcao(&solucao, s, 0.1);
        std::cout << "Multa após a construção: " << novaSolucao.multaSolucao << std::endl;
        melhorSolucao = novaSolucao;

        int iterILS = 0;
        while(iterILS < maxIterILS){
            //std::cout << "Iteração " << i << " do ILS, iteração " << iterILS << " da busca local" << std::endl;
            BuscaLocal(novaSolucao, s);

            // Critério de aceitação: aceita se for melhor
            if (novaSolucao.multaSolucao < melhorSolucao.multaSolucao) {
                melhorSolucao = novaSolucao;
                iterILS = 0;
            }

            DoubleBridge(novaSolucao);
            iterILS++;
        }

         if (novaSolucao.multaSolucao < melhorSolucao.multaSolucao) {
            melhorSolucao = novaSolucao;
            std::cout << "Melhor multa encontrada na iteração " << i << ": " << melhorSolucao.multaSolucao << std::endl;
        }

    }

    solucao = melhorSolucao;

    std::cout << "Melhor ordem encontrada: ";
    for (const auto& pedido : solucao.pedidos) {
        std::cout << pedido.id << " ";
    }
    std::cout << std::endl;

    std::cout << "Melhor multa encontrada: " << solucao.multaSolucao << std::endl;
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
    std::cout << "Multa Inicial: " << solucao.multaSolucao << std::endl;

    int maxIter = 100;
    int maxIterILS = 200;
    ILS(solucao, s, maxIter, maxIterILS);

    return 0;
    
}