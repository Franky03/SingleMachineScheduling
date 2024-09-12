#include "neighbor.h"
#include <algorithm>

double calculateSwapDeltaMulta(Solucao& solucao, std::vector<std::vector<int>>& s, int i, int j){
    double multa_atual = solucao.multa;

    std::swap(solucao.pedidos[i], solucao.pedidos[j]);
    solucao.calcularMulta(s);

    double delta_multa = solucao.multa - multa_atual;

    std::swap(solucao.pedidos[i], solucao.pedidos[j]);
    solucao.calcularMulta(s);

    return delta_multa;
}

bool bestImprovementSwap(Solucao& solucao, std::vector<std::vector<int>>& s){
   double bestDeltaMulta = 0;
   int best_i, best_j;
    
    for (int i = 1; i < solucao.pedidos.size() - 1; i++){
        for (int j = i+1; j < solucao.pedidos.size(); j++){
            // calcular a mudança incremental na multa
            double delta_multa = calculateSwapDeltaMulta(solucao, s, i, j);
            
            if(delta_multa < bestDeltaMulta){
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0){
        std::swap(solucao.pedidos[best_i], solucao.pedidos[best_j]);
        solucao.calcularMulta(s);
        return true;
    }

    return false;
}