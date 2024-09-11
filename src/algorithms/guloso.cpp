#include "guloso.h"
#include <algorithm>

bool compararPorMultaETempo(const Pedido& a, const Pedido& b) {
    if (a.prazo != b.prazo) {
        return a.prazo < b.prazo; // menor prazo
    } else if (a.tempo_producao != b.tempo_producao) {
        return a.tempo_producao < b.tempo_producao; // menor tempo de produção
    }
    return a.multa < b.multa; // menor multa
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