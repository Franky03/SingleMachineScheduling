#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>

#include "config.h"
#include "neighbor.h"
#include "guloso.h"

void BuscaLocal(Solucao& solucao, const Setup& setup);
void DoubleBridge(Solucao &solucao);
void ILS_Opt(Solucao& solucao, const Setup& setup, const std::string& instanceName);

#endif