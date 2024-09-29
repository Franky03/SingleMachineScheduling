#include "neighbor.h"
#include <algorithm>
#include <chrono>

inline void swapKBlocos(Solucao& solucao, int i, int j, int k) {
    for (int l = 0; l < k; l++) {
        std::swap(solucao.pedidos[i + l], solucao.pedidos[j + l]);
    }
}

inline double calcularMultaAcumulada(Solucao& solucao, const Setup& setup, int inicio) {
    double multaAcumulada = (inicio == 0) ? 0 : solucao.multaAcumulada[inicio - 1];
    int tempo_atual = (inicio == 0) ? 0 : solucao.tempoAcumulado[inicio - 1];
    int ultimoPedidoId = (inicio == 0) ? 0 : solucao.pedidos[inicio - 1].id;
    
    for (int l = inicio; l < solucao.pedidos.size(); ++l) {
        int pedidoAtualId = solucao.pedidos[l].id;
        tempo_atual += setup.obterSetup(ultimoPedidoId, pedidoAtualId);
        tempo_atual += solucao.pedidos[l].tempo_producao;
        multaAcumulada += solucao.calcularMultaPedido(tempo_atual, solucao.pedidos[l]);
        ultimoPedidoId = pedidoAtualId;
    }
    
    return multaAcumulada;
}

double InferenciaDeltaMultaSwap(Solucao& solucao, const Setup& setup, int i, int j, int k = 1) {
    double multa_atual = solucao.multaSolucao;
    swapKBlocos(solucao, i, j, k);
    double multa_depois = calcularMultaAcumulada(solucao, setup, std::min(i, j));
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

inline void insertKBlocos(Solucao& solucao, int i, int j, int k){
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
    double multa_atual = solucao.multaSolucao;
    insertKBlocos(solucao, i, j, k);
    double multa_depois = calcularMultaAcumulada(solucao, setup, std::min(i, j));
    insertKBlocos(solucao, j, i, k);
    return multa_depois - multa_atual;
}


bool bestImprovementInsert(Solucao& solucao, const Setup& setup) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    int size = solucao.pedidos.size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;

            Solucao temp_solucao = solucao;

            double delta_multa = InferenciaDeltaMultaInsert(solucao, setup, i, j, 1);

            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        insertKBlocos(solucao, best_i, best_j, 1);
        solucao.calcularMulta(setup); 
        return true;
    }

    return false;
}

bool bestImprovementShift(Solucao& solucao, const Setup& setup, int k) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1, best_k = -1;

    int size = solucao.pedidos.size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int k_final = k;
            if (i == j) continue;
            if (i + k > size) k_final = size - i;

            double delta_multa = InferenciaDeltaMultaInsert(solucao, setup, i, j, k_final);

            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
                best_k = k_final;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        insertKBlocos(solucao, best_i, best_j, best_k);
        solucao.calcularMulta(setup); 
        return true;
    }

    return false;
}

double InferenciaDeltaMulta2Opt(Solucao& solucao, const Setup& setup, int i, int j) {
    double multa_atual = solucao.multaSolucao;
    std::reverse(solucao.pedidos.begin() + i, solucao.pedidos.begin() + j + 1);
    double multa_depois = calcularMultaAcumulada(solucao, setup, std::min(i, j));
    std::reverse(solucao.pedidos.begin() + i, solucao.pedidos.begin() + j + 1);
    return multa_depois - multa_atual;
}


bool bestImprovement2opt(Solucao& solucao, const Setup& setup) {
    double bestDeltaMulta = 0;
    int best_i = -1, best_j = -1;

    for (int i = 1; i < solucao.pedidos.size() - 2; i++) {
        for (int j = i + 1; j < solucao.pedidos.size() - 1; j++) {
            double delta_multa = InferenciaDeltaMulta2Opt(solucao, setup, i, j);
            if (delta_multa < bestDeltaMulta) {
                bestDeltaMulta = delta_multa;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDeltaMulta < 0) {
        std::reverse(solucao.pedidos.begin() + best_i, solucao.pedidos.begin() + best_j + 1);
        solucao.calcularMulta(setup);
        return true;
    }

    return false;
}
