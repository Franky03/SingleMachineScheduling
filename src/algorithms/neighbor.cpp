#include "neighbor.h"
#include <algorithm>
#include <chrono>

void swapKBlocos(Solucao& solucao, int i, int j, int k) {
    for (int l = 0; l < k; l++) {
        std::swap(solucao.pedidos[i + l], solucao.pedidos[j + l]);
    }
}

/* double calculateSwapDeltaMultaK(Solucao &temp_solucao, int i, int j, int k) {
    double multa_atual = temp_solucao.multaSolucao; // 5324
    swapKBlocos(temp_solucao, i, j, k); 
    temp_solucao.calcularMulta(); // 41000
    double multa_after = temp_solucao.multaSolucao;
    swapKBlocos(temp_solucao, i, j, k);
    temp_solucao.multaSolucao = multa_atual; 
    return temp_solucao.multaSolucao - multa_atual; // 41000 - 5324 = 35676 
}
 */
double calculateSwapDeltaMultaK(Solucao &temp_solucao, int i, int j, int k) {
    double multa_atual = temp_solucao.multaSolucao;
    swapKBlocos(temp_solucao, i, j, k); 
    double multa_after = temp_solucao.atualizarMulta(i);
    swapKBlocos(temp_solucao, i, j, k);
    return multa_after - multa_atual;
}

double calcularMultaLocal(Solucao &solucao, int index) {
    double multa_local = 0;
    int tempo_atual = 0;

    if (index == 0) {
        tempo_atual = solucao.s[0][solucao.pedidos[index].id];
    } else {
        tempo_atual = solucao.s[solucao.pedidos[index - 1].id][solucao.pedidos[index].id];
    }

    tempo_atual += solucao.pedidos[index].tempo_producao;
    if (tempo_atual > solucao.pedidos[index].prazo) {
        int atraso = tempo_atual - solucao.pedidos[index].prazo;
        multa_local += atraso * solucao.pedidos[index].multa;
    }

    return multa_local;
}

double calcualarDeltaMultaIncremental(Solucao &solucao, int i, int j){
    // definir os índices dos vizinhos diretos de i e j
    int prev_i = i > 0 ? i - 1 : -1;
    int next_i = i < solucao.pedidos.size() - 1 ? i + 1 : -1;
    int prev_j = j > 0 ? j - 1 : -1;
    int next_j = j < solucao.pedidos.size() - 1 ? j + 1 : -1;

    double multa_atual = calcularMultaLocal(solucao, i) + calcularMultaLocal(solucao, j);

    if (prev_i != -1) multa_atual += calcularMultaLocal(solucao, prev_i);
    if (next_i != -1 && next_i != j) multa_atual += calcularMultaLocal(solucao, next_i);
    if (prev_j != -1 && prev_j != prev_i) multa_atual += calcularMultaLocal(solucao, prev_j);
    if (next_j != -1 && next_j != i) multa_atual += calcularMultaLocal(solucao, next_j);

    std::swap(solucao.pedidos[i], solucao.pedidos[j]);

    double multa_depois = calcularMultaLocal(solucao, i) + calcularMultaLocal(solucao, j);

    if (prev_i != -1) multa_depois += calcularMultaLocal(solucao, prev_i);
    if (next_i != -1 && next_i != j) multa_depois += calcularMultaLocal(solucao, next_i);
    if (prev_j != -1 && prev_j != prev_i) multa_depois += calcularMultaLocal(solucao, prev_j);
    if (next_j != -1 && next_j != i) multa_depois += calcularMultaLocal(solucao, next_j);

    std::swap(solucao.pedidos[i], solucao.pedidos[j]);
    return multa_depois - multa_atual;
}

bool bestImprovementSwap(Solucao& solucao){
   double bestDeltaMulta = 0;
   int best_i, best_j;
    
    for (int i = 0; i < solucao.pedidos.size() - 1; i++){
        for (int j = i+1; j < solucao.pedidos.size(); j++){
            double delta_multa = calcualarDeltaMultaIncremental(solucao, i, j);
            
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

double calculateSwapDeltaMultaKIncremental(Solucao &solucao, int i, int j, int k) {
    double multa_atual = 0, multa_after = 0;

    int prev_i = i > 0 ? i - 1 : -1;
    int next_i = i + k < solucao.pedidos.size() ? i + k : -1;
    int prev_j = j > 0 ? j - 1 : -1;
    int next_j = j + k < solucao.pedidos.size() ? j + k : -1;

    // calcular a multa antes da troca para as posições i, j e seus vizinhos
    if (prev_i != -1) multa_atual += calcularMultaLocal(solucao, prev_i);
    for (int l = i; l < i + k; l++) {
        multa_atual += calcularMultaLocal(solucao, l);
    }
    if (next_i != -1) multa_atual += calcularMultaLocal(solucao, next_i);

    if (prev_j != -1) multa_atual += calcularMultaLocal(solucao, prev_j);
    for (int l = j; l < j + k; l++) {
        multa_atual += calcularMultaLocal(solucao, l);
    }
    if (next_j != -1) multa_atual += calcularMultaLocal(solucao, next_j);

    swapKBlocos(solucao, i, j, k);

    if (prev_i != -1) multa_after += calcularMultaLocal(solucao, prev_i);
    for (int l = i; l < i + k; l++) {
        multa_after += calcularMultaLocal(solucao, l);
    }
    if (next_i != -1) multa_after += calcularMultaLocal(solucao, next_i);

    if (prev_j != -1) multa_after += calcularMultaLocal(solucao, prev_j);
    for (int l = j; l < j + k; l++) {
        multa_after += calcularMultaLocal(solucao, l);
    }
    if (next_j != -1) multa_after += calcularMultaLocal(solucao, next_j);

    swapKBlocos(solucao, i, j, k);

    return multa_after - multa_atual;
}

bool bestImprovementSwapK(Solucao& solucao, int k){
    double bestDeltaMulta = 0;
    int best_i, best_j;

    for (int i = 0; i <= solucao.pedidos.size() - k; i++) {
        for (int j = i + k; j <= solucao.pedidos.size() - k; j++) {
            double delta_multa = calculateSwapDeltaMultaKIncremental(solucao, i, j, k);

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

double calcularDeltaMultaIncrementalInsert(Solucao &solucao, int origem, int destino) {
    double multa_atual = 0, multa_after = 0;

    // identifica os vizinhos das posições de origem e destino
    int prev_origem = origem > 0 ? origem - 1 : -1;
    int next_origem = origem < solucao.pedidos.size() - 1 ? origem + 1 : -1;
    int prev_destino = destino > 0 ? destino - 1 : -1;
    int next_destino = destino < solucao.pedidos.size() ? destino : -1;

    // calcular a multa antes de mover o pedido (na posição original)
    if (prev_origem != -1) multa_atual += calcularMultaLocal(solucao, prev_origem);
    multa_atual += calcularMultaLocal(solucao, origem);
    if (next_origem != -1) multa_atual += calcularMultaLocal(solucao, next_origem);

    // calcular a multa na posição de destino (antes de inserir)
    if (prev_destino != -1) multa_atual += calcularMultaLocal(solucao, prev_destino);
    if (next_destino != -1) multa_atual += calcularMultaLocal(solucao, next_destino);

    // simular a inserção removendo o pedido da origem e inserindo no destino
    Pedido pedido_removido = solucao.pedidos[origem];
    solucao.pedidos.erase(solucao.pedidos.begin() + origem); // Remove na origem

    if (destino > origem) destino--; // ajuste de índice se o destino estiver após a origem

    solucao.pedidos.insert(solucao.pedidos.begin() + destino, pedido_removido); // Insere no destino

    // recalcular a multa após a inserção
    if (prev_origem != -1) multa_after += calcularMultaLocal(solucao, prev_origem);
    if (next_origem != -1 && next_origem != destino) multa_after += calcularMultaLocal(solucao, next_origem);

    if (prev_destino != -1) multa_after += calcularMultaLocal(solucao, prev_destino);
    multa_after += calcularMultaLocal(solucao, destino);
    if (next_destino != -1 && next_destino != origem) multa_after += calcularMultaLocal(solucao, next_destino);

    // reverter as alterações para manter o estado original da solução
    solucao.pedidos.erase(solucao.pedidos.begin() + destino);
    solucao.pedidos.insert(solucao.pedidos.begin() + origem, pedido_removido);

    // retornar a diferença de multa
    return multa_after - multa_atual;
}

bool bestImprovementInsert(Solucao& solucao){
    double bestDeltaMulta  = std::numeric_limits<double>::max();;
    int best_i = -1, best_j = -1;

    int size = solucao.pedidos.size();

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i == j) continue;

            double delta_multa = calcularDeltaMultaIncrementalInsert(solucao, j, i);

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