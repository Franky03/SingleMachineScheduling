#ifndef GULOSO_H
#define GULOSO_H

#include <vector>
#include "config.h"

// Função para ordenar os pedidos com base em um critério guloso (multas, prazos, etc.)
void ordenarPedidos(std::vector<Pedido>& pedidos);

// Função principal do algoritmo guloso
std::vector<Pedido> gulosao(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s);

#endif // GULOSO_H