#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>

#include "config.h"
#include "neighbor.h"
#include "guloso.h"

void BuscaLocal(Solucao& solucao, std::vector<std::vector<int>>& s);
void DoubleBridge(Solucao &solucao);
void ILS(Solucao& solucao, std::vector<std::vector<int>>& s);
void SimulatedAnnealing(Solucao &solucao, std::vector<std::vector<int>>& s);
void ILS_Opt(Solucao& solucao, std::vector<std::vector<int>>& s);

#endif