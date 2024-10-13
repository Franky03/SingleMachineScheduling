#include "guloso.h"
#include "neighbor.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <queue>

struct CompararPedido { // para a fila de prioridade
    bool operator()(const Pedido& a, const Pedido& b) const {
        return a.prazo < b.prazo;
    }
};


Solucao* Construcao(Solucao* solucao, const Setup& setup ,double alpha) {
    std::vector<Pedido> naoAlocados = solucao->pedidos;
    solucao->pedidos.clear();
    
    // complexidade O(n) para construir a fila de prioridade
    std::priority_queue<Pedido, std::vector<Pedido>, CompararPedido> filaPrioridade(naoAlocados.begin(), naoAlocados.end());
    std::vector<Pedido> RCL; // Restricted Candidate List  

    int numPedidos = filaPrioridade.size();
    int limiteEstatico = numPedidos / 3;  // define o ponto de corte

    // processa a primeira metade dos pedidos de forma estática
    for (int i = 0; i < limiteEstatico && !filaPrioridade.empty(); ++i) {
        Pedido pedido = filaPrioridade.top(); // O(1)
        filaPrioridade.pop(); // O(log n)
        solucao->pedidos.push_back(pedido);
    }

    // a partir da metade, processa aleatoriamente
    while (!filaPrioridade.empty()) { // O(n)
        // preenche a RCL com base no alpha, mas apenas para a segunda metade
        while (RCL.size() < std::max(1, static_cast<int>(filaPrioridade.size() * alpha))) { // O(n - n/3) = O(2n/3) = O(n)
            Pedido pedido = filaPrioridade.top(); // O(1)
            filaPrioridade.pop(); // O(log n)
            RCL.push_back(pedido); // O(1)
        }

        // escolhe aleatoriamente a partir da RCL
        int indexEscolhido = rand() % RCL.size();
        Pedido pedidoEscolhido = RCL[indexEscolhido];

        solucao->pedidos.push_back(pedidoEscolhido);

        // remove o escolhido da RCL
        RCL.erase(RCL.begin() + indexEscolhido);
    }

    return solucao;
}

Solucao* gulosao(Solucao* solucao, const Setup& setup) {
    
    std::sort(solucao->pedidos.begin(), solucao->pedidos.end(), [](const Pedido& a, const Pedido& b) {
        return a.multa > b.multa;
    });

    solucao->calcularMulta(setup);

    return solucao;
    
}
