#ifndef GULOSO_H
#define GULOSO_H

#include <vector>
#include "config.h"
// Função principal do algoritmo guloso
Solucao* Construcao(Solucao* solucao, const std::vector<std::vector<int>>& s, double alpha);

#endif // GULOSO_H