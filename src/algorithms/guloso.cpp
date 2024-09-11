#include "guloso.h"
#include <algorithm>

bool compararPorMultaETempo(const Pedido& a, const Pedido& b) {
    if (a.multa != b.multa) {
        return a.multa < b.multa; // multa menor é melhor
    } 

    if (a.prazo != b.prazo) {
        return a.prazo > b.prazo; // prazo maior é melhor
    }

    return a.tempo_producao < b.tempo_producao; // tempo menor é melhor
}

std::vector<Pedido> gulosao(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s) {
    std::vector<Pedido> pedidosOrdenados = pedidos;
    ordenarPedidos(pedidosOrdenados);
    return pedidosOrdenados;
}

// ordena os pedidos de acordo com o critério de menor tempo e menor multa
void ordenarPedidos(std::vector<Pedido>& pedidos) {
    std::sort(pedidos.begin(), pedidos.end(), compararPorMultaETempo);
}