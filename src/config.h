#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>
#include <list>
#include <queue>

using namespace std;

struct Pedido {
    int tempo_producao;
    int prazo;
    int multa;
    int id;
    double multaPedido;
};



struct Solucao {
    vector<Pedido> pedidos;
    int multaSolucao = 0;
    vector<vector<int>> s;

    Solucao() : multaSolucao(0) {}
    Solucao(const vector<Pedido> &pedidos, int multa, vector<vector<int>> setup) : pedidos(pedidos), multaSolucao(multa), s(setup){}

    void calcularMulta() {
        multaSolucao = 0;
        int tempo_atual = 0;

        tempo_atual += s[0][pedidos[0].id];
        tempo_atual += pedidos[0].tempo_producao;

        if (tempo_atual > pedidos[0].prazo) {
            int atraso = tempo_atual - pedidos[0].prazo;
            multaSolucao += atraso * pedidos[0].multa;
        }

        for (int i = 1; i < pedidos.size(); i++) {
            // adiciona o tempo de setup entre o pedido anterior e o atual
            tempo_atual += s[pedidos[i - 1].id][pedidos[i].id];
            tempo_atual += pedidos[i].tempo_producao;

            if (tempo_atual > pedidos[i].prazo) {
                int atraso = tempo_atual - pedidos[i].prazo;
                multaSolucao += atraso * pedidos[i].multa;
            }
        }
    }    
    
};


#endif // CONFIG_H