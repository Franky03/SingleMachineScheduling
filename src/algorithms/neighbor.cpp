#include "neighbor.h"
#include <algorithm>

void swapKBlocos(Solucao& solucao, int i, int j, int k) {
    for (int l = 0; l < k; l++) {
        std::swap(solucao.pedidos[i + l], solucao.pedidos[j + l]);
    }
}

double calculateSwapDeltaMultaK(Solucao &temp_solucao, int i, int j, int k) {
    double multa_atual = temp_solucao.multaSolucao;
    swapKBlocos(temp_solucao, i, j, k);
    temp_solucao.calcularMulta();
    double multa_after = temp_solucao.multaSolucao;
    swapKBlocos(temp_solucao, i, j, k);
    temp_solucao.multaSolucao = multa_atual;
    return temp_solucao.multaSolucao - multa_atual;
}

bool bestImprovementSwap(Solucao& solucao){
   double bestDeltaMulta = 0;
   int best_i, best_j;
    
    for (int i = 0; i < solucao.pedidos.size() - 1; i++){
        for (int j = i+1; j < solucao.pedidos.size(); j++){
            double delta_multa = calculateSwapDeltaMultaK(solucao, i, j, 1);
            
            if(delta_multa < bestDeltaMulta){
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0){
        std::swap(solucao.pedidos[best_i], solucao.pedidos[best_j]);
        solucao.multaSolucao += bestDeltaMulta;
        return true;
    }

    return false;
}

bool bestImprovementSwapK(Solucao& solucao, int k){
    double bestDeltaMulta = 0;
    int best_i, best_j;

    for (int i = 0; i <= solucao.pedidos.size() - k; i++) {
        for (int j = i + k; j <= solucao.pedidos.size() - k; j++) {
            double delta_multa = calculateSwapDeltaMultaK(solucao, i, j, k);
            
            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        swapKBlocos(solucao, best_i, best_j, k);
        solucao.multaSolucao += bestDeltaMulta;
        return true;
    }

    return false;

}

bool bestImprovementInsert(Solucao& solucao){
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    int size = solucao.pedidos.size();

    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size; j++){
            if(i == j) continue;

            double multa_atual = solucao.multaSolucao;

            Solucao temp_solucao = solucao;

            std::vector<Pedido> pedidos = temp_solucao.pedidos;
            Pedido pedido_j = pedidos[j];
            pedidos.erase(pedidos.begin() + j); // Remove o pedido j
            pedidos.insert(pedidos.begin() + i, pedido_j); // Insere o pedido j na posição i

            temp_solucao.pedidos = pedidos;
            temp_solucao.calcularMulta();

            double delta_multa = temp_solucao.multaSolucao - multa_atual;

            if(delta_multa < bestDeltaMulta){
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }


    if(bestDeltaMulta < 0){
        Pedido pedido_j = solucao.pedidos[best_j];
        solucao.pedidos.erase(solucao.pedidos.begin() + best_j);
        solucao.pedidos.insert(solucao.pedidos.begin() + best_i, pedido_j);
        solucao.multaSolucao += bestDeltaMulta;
        return true;
    }

    return false;
}

bool bestImprovementReinsertion(Solucao& solucao, int k){
    int size = solucao.pedidos.size();
    // escolher um start aleatório
    int start = rand() % size;
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    for(int i = 0; i < size; i++){
        Solucao temp_solucao = solucao;

        std::vector<Pedido> bloco(temp_solucao.pedidos.begin() + i, temp_solucao.pedidos.begin() + i + k);
        temp_solucao.pedidos.erase(temp_solucao.pedidos.begin() + i, temp_solucao.pedidos.begin() + i + k);

        for (int j = 0; j <= temp_solucao.pedidos.size(); j++) {
            if (j == i) continue;

            std::vector<Pedido> pedidos = temp_solucao.pedidos;
            pedidos.insert(pedidos.begin() + j, bloco.begin(), bloco.end());

            temp_solucao.pedidos = pedidos;
            
            double multa_atual = solucao.multaSolucao;
            temp_solucao.calcularMulta();
            double delta_multa = temp_solucao.multaSolucao - multa_atual;

            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }

    }

    if (bestDeltaMulta < 0) {
        std::vector<Pedido> bloco(solucao.pedidos.begin() + best_i, solucao.pedidos.begin() + best_i + k);
        solucao.pedidos.erase(solucao.pedidos.begin() + best_i, solucao.pedidos.begin() + best_i + k);

        if (best_j > best_i) best_j -= k;

        solucao.pedidos.insert(solucao.pedidos.begin() + best_j, bloco.begin(), bloco.end());
        solucao.multaSolucao += bestDeltaMulta;
        return true;
    }

    return false;
}

bool bestImprovement2opt(Solucao& solucao){
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    for (int i = 1; i < solucao.pedidos.size() - 2; i++) {
        for (int j = i + 1; j < solucao.pedidos.size() - 1; j++) {
            double multa_atual = solucao.multaSolucao;

            Solucao temp_solucao = solucao;
            std::vector<Pedido> pedidos = temp_solucao.pedidos;
            std::reverse(pedidos.begin() + i, pedidos.begin() + j + 1); // Inverte o segmento

            temp_solucao.pedidos = pedidos;
            temp_solucao.calcularMulta();

            double delta_multa = temp_solucao.multaSolucao - multa_atual;
            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        std::reverse(solucao.pedidos.begin() + best_i, solucao.pedidos.begin() + best_j + 1);
        solucao.multaSolucao += bestDeltaMulta;
        return true;
    }

    return false;
}

bool bestImprovementOrOpt(Solucao& solucao, int k){
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    int size = solucao.pedidos.size();

    for (int i = 0; i <= size - k; i++) {
        for (int j = 0; j <= size - k; j++) {
            if (i == j || (i < j && i + k > j)) continue; // evita sobreposição de índices

            double multa_atual = solucao.multaSolucao;

            Solucao temp_solucao = solucao;

            std::vector<Pedido> pedidos = temp_solucao.pedidos;
            std::vector<Pedido> bloco(pedidos.begin() + j, pedidos.begin() + j + k); // Bloco de tamanho k

            pedidos.erase(pedidos.begin() + j, pedidos.begin() + j + k);
            pedidos.insert(pedidos.begin() + i, bloco.begin(), bloco.end());

            temp_solucao.pedidos = pedidos;
            temp_solucao.calcularMulta();

            double delta_multa = temp_solucao.multaSolucao - multa_atual;

            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestDeltaMulta < 0){
        std::vector<Pedido> bloco(solucao.pedidos.begin() + best_j, solucao.pedidos.begin() + best_j + k);
        solucao.pedidos.erase(solucao.pedidos.begin() + best_j, solucao.pedidos.begin() + best_j + k);
        solucao.pedidos.insert(solucao.pedidos.begin() + best_i, bloco.begin(), bloco.end());
        solucao.multaSolucao += bestDeltaMulta;
        return true;
    }

    return false;
}