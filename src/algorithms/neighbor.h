#ifndef VIZINHANCA_H
#define VIZINHANCA_H

#include <vector>
#include "config.h"

// implementação dos movimentos de vizinhança do algoritmo de busca local, que são:
// 1. Troca de pedidos de posição (swap)
// 2. Inserção de um pedido em uma posição diferente (insert)
// 3. Reversão de sub-sequência de pedidos (2-opt)

bool bestImprovementSwap(Solucao& solucao, std::vector<std::vector<int>>& s);
bool bestImprovementInsert(Solucao& solucao, std::vector<std::vector<int>>& s);
bool bestImprovement2opt(Solucao& solucao, std::vector<std::vector<int>>& s); 

#endif // VIZINHANCA_H