#ifndef GULOSO_H
#define GULOSO_H

#include <vector>
#include "config.h"

// Função para ordenar os pedidos com base em um critério guloso (multas, prazos, etc.)
void ordenarPedidosMultas(std::vector<Pedido>& pedidos);
void ordenarPedidosPrazos(std::vector<Pedido>& pedidos);
void ordenarPedidosTempo(std::vector<Pedido>& pedidos);
void ordenarPedidosMultaPrazo(std::vector<Pedido>& pedidos);

// Função principal do algoritmo guloso
std::vector<Pedido> gulosao_multa(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s);
std::vector<Pedido> gulosao_prazo(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s);
std::vector<Pedido> gulosao_tempo(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s);
std::vector<Pedido> gulosao_multa_prazo(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s);

#endif // GULOSO_H