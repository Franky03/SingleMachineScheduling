#ifndef VIZINHANCA_H
#define VIZINHANCA_H

#include <vector>
#include "config.h"

bool bestImprovementSwap(Solucao& solucao, const Setup& setup);
bool bestImprovementInsert(Solucao& solucao, const Setup& setup);
bool bestImprovement2opt(Solucao& solucao, const Setup& setup);
bool bestImprovementSwapK(Solucao& solucao, const Setup& setup, int k);
bool bestImprovementShift(Solucao& solucao, const Setup& setup, int k);
void allImprovementSwap(Solucao& solucao, const Setup& setup);
void allImprovementShift(Solucao& solucao, const Setup& setup, int k);
void allImprovement2opt(Solucao& solucao, const Setup& setup);
bool bestImprovementReverse(Solucao& solucao, const Setup& setup, int k);

#endif // VIZINHANCA_H