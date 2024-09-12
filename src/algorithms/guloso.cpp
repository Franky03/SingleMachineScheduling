#include "guloso.h"
#include <algorithm>

Solucao* gulosao(Solucao* solucao, const std::vector<std::vector<int>>& s) {
    std::sort(solucao->pedidos.begin(), solucao->pedidos.end(), [](const Pedido& a, const Pedido& b) {
        if(a.prazo != b.prazo) {
            return a.prazo < b.prazo; // menor prazo
        } else if(a.tempo_producao != b.tempo_producao) {
            return a.tempo_producao < b.tempo_producao; // menor tempo de produção
        }
        return a.multa < b.multa; // menor multa
    });

    solucao->calcularMulta(s);
    return solucao;
}
