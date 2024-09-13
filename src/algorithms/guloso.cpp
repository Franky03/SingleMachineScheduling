#include "guloso.h"
#include <algorithm>

Solucao *Construcao(Solucao* solucao, const std::vector<std::vector<int>>& s, double alpha){
    std::vector<Pedido> naoAlocados = solucao->pedidos;
    solucao->pedidos.clear();

    while(!naoAlocados.empty()){
        std::sort(naoAlocados.begin(), naoAlocados.end(), [](const Pedido& a, const Pedido& b) {
            return (a.prazo - a.tempo_producao) / a.multa < (b.prazo - b.tempo_producao) / b.multa;
        });

        int rclSize = std::max(1, static_cast<int>(naoAlocados.size() * alpha));
        int indexEscolhido = rand() % rclSize;

        Pedido pedidoEscolhido = naoAlocados[indexEscolhido];
        solucao->pedidos.push_back(pedidoEscolhido);
        naoAlocados.erase(naoAlocados.begin() + indexEscolhido);

    }

    solucao->calcularMulta(s);  
    return solucao;
}

Solucao* gulosao(Solucao* solucao, const std::vector<std::vector<int>>& s) {
    // fazer a construção iterativamente até que todos os pedidos sejam alocados

    std::sort(solucao->pedidos.begin(), solucao->pedidos.end(), [](const Pedido& a, const Pedido& b) {
        if(a.multa != b.multa) {
            return a.multa < b.multa; // menor prazo
        } else if(a.tempo_producao != b.tempo_producao) {
            return a.tempo_producao < b.tempo_producao; // menor tempo de produção
        }
        return a.prazo < b.prazo; // menor multa
    });

    solucao->calcularMulta(s);
    return solucao;
}
