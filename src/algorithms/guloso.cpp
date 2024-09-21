#include "guloso.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <queue>

struct CompararPedido { // para a fila de prioridade
    bool operator()(const Pedido& a, const Pedido& b) const {
        return a.multa / (a.prazo - a.tempo_producao) < b.multa / (b.prazo - b.tempo_producao);
    }
};


Solucao *Construcao(Solucao* solucao, double alpha){
    std::vector<Pedido> naoAlocados = solucao->pedidos;
    solucao->pedidos.clear();
    // armazena os pedidos não alocados em uma fila de prioridade de acordo com a razão multa/atraso
    std::priority_queue<Pedido, std::vector<Pedido>, CompararPedido> filaPrioridade(naoAlocados.begin(), naoAlocados.end());
    std::vector<Pedido> RCL;
    while (!filaPrioridade.empty()) {
        while (RCL.size() < std::max(1, static_cast<int>(filaPrioridade.size() * alpha))) {
            Pedido pedido = filaPrioridade.top();
            filaPrioridade.pop();
            RCL.push_back(pedido);
        }
        int indexEscolhido = rand() % RCL.size();
        Pedido pedidoEscolhido = RCL[indexEscolhido];

        solucao->pedidos.push_back(pedidoEscolhido);

        RCL.erase(RCL.begin() + indexEscolhido);
    }
    solucao->calcularMulta();  
    return solucao;
}

Solucao* gulosao(Solucao* solucao) {
    
    std::sort(solucao->pedidos.begin(), solucao->pedidos.end(), [](const Pedido& a, const Pedido& b) {
        return a.multa > b.multa;
    });

    solucao->calcularMulta();

    return solucao;
    
}
