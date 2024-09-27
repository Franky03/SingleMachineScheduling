#ifndef VIZINHANCA_H
#define VIZINHANCA_H

#include <vector>
#include "config.h"

// implementação dos movimentos de vizinhança do algoritmo de busca local, que são:
// 1. Troca de pedidos de posição (swap)a
// 2. Inserção de um pedido em uma posição diferente (insert)
// 3. Reversão de sub-sequência de pedidos (2-opt)

bool bestImprovementSwap(Solucao& solucao, const Setup& setup);
bool bestImprovementInsert(Solucao& solucao, const Setup& setup);
bool bestImprovement2opt(Solucao& solucao, const Setup& setup);
bool bestImprovementSwapK(Solucao& solucao, const Setup& setup, int k);
bool bestImprovementShift(Solucao& solucao, const Setup& setup, int k);

#endif // VIZINHANCA_H