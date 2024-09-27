#ifndef GULOSO_H
#define GULOSO_H

#include <vector>
#include "config.h"
// Função principal do algoritmo guloso
Solucao* Construcao(Solucao* solucao, const Setup &setup,double alpha);
Solucao* gulosao(Solucao* solucao, const Setup &setup);

#endif // GULOSO_H