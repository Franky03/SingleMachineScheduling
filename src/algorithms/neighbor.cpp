#include "neighbor.h"
#include <algorithm>

double calculateSwapDeltaMultaOpt(Solucao temp_solucao, int i, int j) {
    double multa_atual = temp_solucao.multaSolucao;
    std::swap(temp_solucao.pedidos[i], temp_solucao.pedidos[j]);
    temp_solucao.calcularMulta();
    return temp_solucao.multaSolucao - multa_atual;
}

bool bestImprovementSwap(Solucao& solucao){
   double bestDeltaMulta = 0;
   int best_i, best_j;
    
    for (int i = 0; i < solucao.pedidos.size() - 1; i++){
        for (int j = i+1; j < solucao.pedidos.size(); j++){
            double delta_multa = calculateSwapDeltaMultaOpt(solucao, i, j);
            
            if(delta_multa < bestDeltaMulta){
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0){
        std::swap(solucao.pedidos[best_i], solucao.pedidos[best_j]);
        solucao.calcularMulta();
        return true;
    }

    return false;
}

void swapKBlocos(Solucao& solucao, int i, int j, int k) {
    for (int l = 0; l < k; l++) {
        std::swap(solucao.pedidos[i + l], solucao.pedidos[j + l]);
    }
}

double calculateSwapDeltaMultaK(Solucao temp_solucao, int i, int j, int k) {
    double multa_atual = temp_solucao.multaSolucao;
    swapKBlocos(temp_solucao, i, j, k);
    temp_solucao.calcularMulta();
    return temp_solucao.multaSolucao - multa_atual;
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
        solucao.calcularMulta();
        return true;
    }

    return false;

}

bool bestImprovementKOpt(Solucao& solucao, int k){
    double bestDeltaMulta = 0;
    int best_i, best_j;
    
    for (int i = 0; i < solucao.pedidos.size() - k; i++){
        for (int j = i+k; j < solucao.pedidos.size(); j++){
            // calcular a mudança incremental na multa
            double multa_atual = solucao.multaSolucao;

            Solucao temp_solucao = solucao;
            std::reverse(temp_solucao.pedidos.begin() + i, temp_solucao.pedidos.begin() + j + 1);

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
        solucao.calcularMulta();
        return true;
    }

    return false;
}

bool bestImprovementInsert(Solucao& solucao){
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    int size = solucao.pedidos.size();

    for(int i = 0; i < size - 1; i++){
        for(int j = 1; j < size; j++){
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
        solucao.calcularMulta();
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
        solucao.calcularMulta();
        return true;
    }

    return false;
}

bool bestImprovementOrOpt(Solucao& solucao, int tamanho_bloco) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    for (int i = 0; i < solucao.pedidos.size() - tamanho_bloco; i++) {
        for (int j = 0; j < solucao.pedidos.size(); j++) {
            if (j >= i && j <= i + tamanho_bloco) {
                // evita mover o bloco para dentro de si mesmo
                continue;
            }

            // cria uma nova solução temporária
            Solucao temp_solucao = solucao;

            // remove a subsequência de tamanho_bloco da posição i
            
            std::vector<Pedido> subsequencia(temp_solucao.pedidos.begin() + i, temp_solucao.pedidos.begin() + i + tamanho_bloco);
            temp_solucao.pedidos.erase(temp_solucao.pedidos.begin() + i, temp_solucao.pedidos.begin() + i + tamanho_bloco);

            if (j > i) {
                j -= tamanho_bloco;
            }

            // Insere a subsequência na posição j
            temp_solucao.pedidos.insert(temp_solucao.pedidos.begin() + j, subsequencia.begin(), subsequencia.end());

            // Recalcula a penalidade para a nova solução
            temp_solucao.calcularMulta();
            double delta_multa = temp_solucao.multaSolucao - solucao.multaSolucao;

            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        std::vector<Pedido> subsequencia(solucao.pedidos.begin() + best_i, solucao.pedidos.begin() + best_i + tamanho_bloco);
        solucao.pedidos.erase(solucao.pedidos.begin() + best_i, solucao.pedidos.begin() + best_i + tamanho_bloco);
        solucao.pedidos.insert(solucao.pedidos.begin() + best_j, subsequencia.begin(), subsequencia.end());
        solucao.calcularMulta();
        return true;
    }

    return false;
}