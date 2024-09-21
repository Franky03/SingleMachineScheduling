#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>

#include "config.h"
#include "neighbor.h"
#include "guloso.h"

void BuscaLocal(Solucao& solucao);
void DoubleBridge(Solucao &solucao);
void ILS(Solucao& solucao);
void SimulatedAnnealing(Solucao &solucao);
void ILS_Opt(Solucao& solucao);

#endif