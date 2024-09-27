#include "neighbor.h"
#include <algorithm>
#include <chrono>

void swapKBlocos(Solucao& solucao, int i, int j, int k) {
    for (int l = 0; l < k; l++) {
        std::swap(solucao.pedidos[i + l], solucao.pedidos[j + l]);
    }
}

double InferenciaDeltaMultaSwap(Solucao& solucao, const Setup& setup, int i, int j, int k = 1) {
    double multa_atual = solucao.multaSolucao;
    double multaAnterior = (i == 0) ? 0 : solucao.multaPedidos[i - 1];
    int tempoAnterior = (i == 0) ? 0 : solucao.tempoAcumulado[i - 1];
    int tempo_atual = tempoAnterior;
    int final_i = solucao.pedidos.size();

    // Faz o swap temporário dos blocos
    swapKBlocos(solucao, i, j, k);

    double multa_depois = multaAnterior;

    for (int l = i; l < final_i; l++) {
        if (l == 0) {
            tempo_atual += setup.obterSetup(0, solucao.pedidos[l].id);
        } else {
            tempo_atual += setup.obterSetup(solucao.pedidos[l - 1].id, solucao.pedidos[l].id);
        }
        tempo_atual += solucao.pedidos[l].tempo_producao;
        multa_depois += solucao.calcularMultaPedido(tempo_atual, solucao.pedidos[l]);
    }

    // Desfaz o swap temporário
    swapKBlocos(solucao, i, j, k);

    return multa_depois - multa_atual;
}

bool bestImprovementSwap(Solucao& solucao, const Setup& setup) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    for (int i = 0; i < solucao.pedidos.size() - 1; i++) {
        for (int j = i + 1; j < solucao.pedidos.size(); j++) {
            double delta_multa = InferenciaDeltaMultaSwap(solucao, setup, i, j, 1);
            
            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        swapKBlocos(solucao, best_i, best_j, 1);
        solucao.calcularMulta(setup);
        return true;
    }

    return false;
}

bool bestImprovementSwapK(Solucao& solucao, const Setup& setup, int k) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    for (int i = 0; i <= solucao.pedidos.size() - k; i++) {
        for (int j = i + k; j <= solucao.pedidos.size() - k; j++) {
            double delta_multa = InferenciaDeltaMultaSwap(solucao, setup, i, j, k);

            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        swapKBlocos(solucao, best_i, best_j, k);
        solucao.calcularMulta(setup);
        return true;
    }

    return false;
}

void insertKBlocos(Solucao& solucao, int i, int j, int k){
    int n = solucao.pedidos.size();
    
    if(i+k > n) k = n-i;
    if(j+k > n) k = n-j;

    std::vector<Pedido> bloco(solucao.pedidos.begin() + i, solucao.pedidos.begin() + i + k);

    if (j > i) {  // Movendo para a direita
        for (int idx = i; idx < j; ++idx) {
            if (idx + k < n) {
                solucao.pedidos[idx] = solucao.pedidos[idx + k];
            }
        }
    } else if (j < i) {  // Movendo para a esquerda
        for (int idx = i + k - 1; idx >= j; --idx) {
            if (idx - k >= 0) {
                solucao.pedidos[idx] = solucao.pedidos[idx - k];
            }
        }
    }

    for (int idx = 0; idx < k; ++idx) {
        solucao.pedidos[j + idx] = bloco[idx];
    }

}


double InferenciaDeltaMultaInsert(Solucao &solucao, const Setup& setup, int i, int j, int k = 1) {
    double multaAnterior = (i == 0) ? 0 : solucao.multaPedidos[i - 1];
    double multa_atual = solucao.multaSolucao;
    int tempoAnterior = (i == 0) ? 0 : solucao.tempoAcumulado[i - 1]; 
    int tempo_atual = tempoAnterior;
    int final_i = solucao.pedidos.size();

    insertKBlocos(solucao, i, j, k);  // Insere os blocos temporariamente

    double multa_depois = multaAnterior;
    int inicio = (j < i) ? j : i;

    for (int l = inicio; l < final_i; l++) {
        if (l == 0) {
            tempo_atual += setup.obterSetup(0, solucao.pedidos[l].id);
        } else {
            tempo_atual += setup.obterSetup(solucao.pedidos[l - 1].id, solucao.pedidos[l].id);
        }
        tempo_atual += solucao.pedidos[l].tempo_producao;

        if (tempo_atual > solucao.pedidos[l].prazo) {
            int atraso = tempo_atual - solucao.pedidos[l].prazo;
            multa_depois += atraso * solucao.pedidos[l].multa;
        }
    }

    insertKBlocos(solucao, j, i, k);  // Desfaz a inserção temporária

    return multa_depois - multa_atual;
}


bool bestImprovementInsert(Solucao& solucao, const Setup& setup) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    int size = solucao.pedidos.size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;

            double delta_multa = InferenciaDeltaMultaInsert(solucao, setup, i, j, 1);

            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        double multa_atual = solucao.multaSolucao;
        insertKBlocos(solucao, best_i, best_j, 1);
        solucao.calcularMulta(setup); 
        double multa_depois = solucao.multaSolucao;
        if (multa_depois > multa_atual) {
            std::cout << "Multa calculada Antes INSERT: " << multa_atual << " - Multa Depois: " << multa_depois << std::endl;
        }
        return true;
    }

    return false;
}

bool bestImprovementShift(Solucao& solucao, const Setup& setup, int k) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1, k_final = k;

    int size = solucao.pedidos.size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (i + k > size) k_final = size - i;

            double delta_multa = InferenciaDeltaMultaInsert(solucao, setup, i, j, k_final);

            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        double multa_atual = solucao.multaSolucao;
        insertKBlocos(solucao, best_i, best_j, k_final);
        solucao.calcularMulta(setup); 
        double multa_depois = solucao.multaSolucao;

        if (multa_depois > multa_atual) {
            std::cout << "Multa calculada Antes SHIFT: " << multa_atual << " - Multa Depois: " << multa_depois << std::endl;
        }
        return true;
    }

    return false;
}


bool bestImprovement2opt(Solucao& solucao, const Setup& setup) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    for (int i = 1; i < solucao.pedidos.size() - 2; i++) {
        for (int j = i + 1; j < solucao.pedidos.size() - 1; j++) {
            double multa_atual = solucao.multaSolucao;

            Solucao temp_solucao = solucao;
            std::vector<Pedido> pedidos = temp_solucao.pedidos;
            std::reverse(pedidos.begin() + i, pedidos.begin() + j + 1); // Inverte o segmento

            temp_solucao.pedidos = pedidos;
            temp_solucao.calcularMulta(setup);

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
