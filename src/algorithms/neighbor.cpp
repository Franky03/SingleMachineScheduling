#include "neighbor.h"
#include <algorithm>
#include <chrono>

void swapKBlocos(Solucao& solucao, int i, int j, int k) {
    for (int l = 0; l < k; l++) {
        std::swap(solucao.pedidos[i + l], solucao.pedidos[j + l]);
    }
}

double calculateSwapDeltaMultaK(Solucao &temp_solucao, int i, int j, int k) {
    double multa_atual = temp_solucao.multaSolucao;
    swapKBlocos(temp_solucao, i, j, k); 
    double multa_after = temp_solucao.atualizarMulta(i);
    swapKBlocos(temp_solucao, i, j, k);
    return multa_after - multa_atual;
}

double InferenciaDeltaMultaSwap(Solucao& solucao, int i, int j, int k=1){
    // calcula a multa atual para os pedidos i até o pedido i+5
    double multa_atual = solucao.multaSolucao;
    double multaAnterior = (i == 0) ? 0 : solucao.multaPedidos[i - 1];
    int tempoAnterior = (i == 0) ? 0 : solucao.tempoAcumulado[i - 1];
    int tempo_atual = tempoAnterior;
    int final_i =  solucao.pedidos.size() ;
    
    // faz o swap
    swapKBlocos(solucao, i, j, k);

    double multa_depois = multaAnterior;

    for (int l = i; l < final_i; l++) {
        if(l == 0){
            tempo_atual += solucao.s[0][solucao.pedidos[l].id];
        } else {
            tempo_atual += solucao.s[solucao.pedidos[l - 1].id][solucao.pedidos[l].id];
        }
        tempo_atual += solucao.pedidos[l].tempo_producao;

        if (tempo_atual > solucao.pedidos[l].prazo) {
            int atraso = tempo_atual - solucao.pedidos[l].prazo;
            multa_depois += atraso * solucao.pedidos[l].multa;
        }
    }

    // desfaz o swap

    swapKBlocos(solucao, i, j, k);

    return multa_depois - multa_atual;
    
}

bool bestImprovementSwap(Solucao& solucao){
   double bestDeltaMulta = 0;
   int best_i, best_j;
    
    for (int i = 0; i < solucao.pedidos.size() - 1; i++){
        for (int j = i+1; j < solucao.pedidos.size(); j++){
            double delta_multa = InferenciaDeltaMultaSwap(solucao, i, j, 1);
            
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

bool bestImprovementSwapK(Solucao& solucao, int k){
    double bestDeltaMulta = 0;
    int best_i, best_j;

    for (int i = 0; i <= solucao.pedidos.size() - k; i++) {
        for (int j = i + k; j <= solucao.pedidos.size() - k; j++) {
            double delta_multa = InferenciaDeltaMultaSwap(solucao, i, j, k);

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

void insertKBlocos(Solucao& solucao, int i, int j, int k=1){
    if(i == j) return;
    if(i +k > solucao.pedidos.size()){
        k = solucao.pedidos.size() - i;
    }

    std::vector<Pedido> bloco(solucao.pedidos.begin() + i, solucao.pedidos.begin() + i + k);
    solucao.pedidos.erase(solucao.pedidos.begin() + i, solucao.pedidos.begin() + i + k);

    if(j > i){
        j -= k;
    }

    solucao.pedidos.insert(solucao.pedidos.begin() + j, bloco.begin(), bloco.end());
}


double InferenciaDeltaMultaInsert(Solucao &solucao, int i, int j, int k = 1) {
    double multaAnterior = (i == 0) ? 0 : solucao.multaPedidos[i - 1];
    double multa_atual = solucao.multaSolucao;
    int tempoAnterior = (i == 0) ? 0 : solucao.tempoAcumulado[i - 1]; 
    int tempo_atual = tempoAnterior;
    int final_i = solucao.pedidos.size();

    insertKBlocos(solucao, i, j, k);

    double multa_depois = multaAnterior;

    for (int l = i; l < final_i; l++) {
        if (l == 0) {
            tempo_atual += solucao.s[0][solucao.pedidos[l].id];
        } else {
            tempo_atual += solucao.s[solucao.pedidos[l - 1].id][solucao.pedidos[l].id];
        }
        tempo_atual += solucao.pedidos[l].tempo_producao;

        if (tempo_atual > solucao.pedidos[l].prazo) {
            int atraso = tempo_atual - solucao.pedidos[l].prazo;
            multa_depois += atraso * solucao.pedidos[l].multa;
        }
    }

    insertKBlocos(solucao, j, i, k); 

    return multa_depois - multa_atual;
}


bool bestImprovementInsert(Solucao& solucao){
    double bestDeltaMulta  = 0;
    int best_i = -1, best_j = -1;

    int size = solucao.pedidos.size();

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i == j) continue;

            double delta_multa = InferenciaDeltaMultaInsert(solucao, i, j, 1);

            if(delta_multa < bestDeltaMulta){
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }


    if(bestDeltaMulta < 0){
        insertKBlocos(solucao, best_i, best_j, 1); 
        solucao.calcularMulta(); 
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

bool bestImprovementOrOpt(Solucao& solucao, int k) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    int size = solucao.pedidos.size();

    for (int i = 0; i <= size - k; i++) {
        for (int j = 0; j <= size - k; j++) {
            if (i == j || (i < j && i + k > j)) continue; // Evita sobreposição de índices

            double multa_atual = solucao.multaSolucao;
            double multa_parcial_antes = solucao.calcularMultaParcial(std::min(i, j));
            std::vector<Pedido> bloco(solucao.pedidos.begin() + j, solucao.pedidos.begin() + j + k);
            solucao.pedidos.erase(solucao.pedidos.begin() + j, solucao.pedidos.begin() + j + k);
            int nova_posicao = (i < j) ? i : i - k;
            solucao.pedidos.insert(solucao.pedidos.begin() + nova_posicao, bloco.begin(), bloco.end());

            double multa_parcial_depois = solucao.calcularMultaParcial(std::min(nova_posicao, j));
            double multa_depois = multa_atual - multa_parcial_antes + multa_parcial_depois;
            double delta_multa = multa_depois - multa_atual;

            solucao.pedidos.erase(solucao.pedidos.begin() + nova_posicao, solucao.pedidos.begin() + nova_posicao + k);
            solucao.pedidos.insert(solucao.pedidos.begin() + j, bloco.begin(), bloco.end());

            // Verifica se é a melhor melhora até o momento
            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    // Se encontrar uma melhora
    if (bestDeltaMulta < 0) {
        // Mover o bloco da posição best_j para best_i
        std::vector<Pedido> bloco(solucao.pedidos.begin() + best_j, solucao.pedidos.begin() + best_j + k);
        solucao.pedidos.erase(solucao.pedidos.begin() + best_j, solucao.pedidos.begin() + best_j + k);
        int nova_posicao = (best_i < best_j) ? best_i : best_i - k;
        solucao.pedidos.insert(solucao.pedidos.begin() + nova_posicao, bloco.begin(), bloco.end());

        // Atualizar a multa com a melhor melhoria encontrada
        solucao.multaSolucao += bestDeltaMulta;
        return true;
    }

    return false;
}