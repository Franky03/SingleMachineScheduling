#include "guloso.h"
#include <algorithm>

bool compararPorMulta(const Pedido& a, const Pedido& b) {
    return a.multa < b.multa; // multa menor é melhor
}

bool compararPorTempo(const Pedido& a, const Pedido& b) {
    return a.tempo_producao < b.tempo_producao;
}

bool compararPorPrazo(const Pedido& a, const Pedido& b) {
    return a.prazo < b.prazo;  // prazo menor é melhor
}

bool compararPorMultaPrazo(const Pedido& a, const Pedido& b) {
    if (a.multa == b.multa) {
        return a.prazo < b.prazo; // prazo menor é melhor
    }
    return a.multa < b.multa;
}

std::vector<Pedido> gulosao_multa(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s) {
    std::vector<Pedido> pedidosOrdenados = pedidos;
    ordenarPedidosMultas(pedidosOrdenados);
    return pedidosOrdenados;
}

std::vector<Pedido> gulosao_prazo(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s) {
    std::vector<Pedido> pedidosOrdenados = pedidos;
    ordenarPedidosPrazos(pedidosOrdenados);
    return pedidosOrdenados;
}

std::vector<Pedido> gulosao_tempo(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s) {
    std::vector<Pedido> pedidosOrdenados = pedidos;
    ordenarPedidosTempo(pedidosOrdenados);
    return pedidosOrdenados;
}

std::vector<Pedido> gulosao_multa_prazo(const std::vector<Pedido>& pedidos, const std::vector<std::vector<int>>& s) {
    std::vector<Pedido> pedidosOrdenados = pedidos;
    ordenarPedidosMultaPrazo(pedidosOrdenados);
    return pedidosOrdenados;
}

// ordena os pedidos de acordo com o critério de menor multa
void ordenarPedidosMultas(std::vector<Pedido>& pedidos) {
    std::sort(pedidos.begin(), pedidos.end(), compararPorMulta);
}

// ordena os pedidos de acordo com o critério de menor prazo
void ordenarPedidosPrazos(std::vector<Pedido>& pedidos) {
    std::sort(pedidos.begin(), pedidos.end(), compararPorPrazo);
}

// ordena os pedidos de acordo com o critério de menor tempo
void ordenarPedidosTempo(std::vector<Pedido>& pedidos) {
    std::sort(pedidos.begin(), pedidos.end(), compararPorTempo);
}

// ordena os pedidos de acordo com o critério de menor multa e menor prazo
void ordenarPedidosMultaPrazo(std::vector<Pedido>& pedidos) {
    std::sort(pedidos.begin(), pedidos.end(), compararPorMultaPrazo);
}