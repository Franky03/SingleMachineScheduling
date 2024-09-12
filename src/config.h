#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Pedido {
    int tempo_producao;
    int prazo;
    int multa;
    int id;
    bool operator<(const Pedido &p) const {
        return prazo < p.prazo;
    }
};

struct Solucao {
    vector<Pedido> pedidos;
    int multa;

    Solucao() : multa(0) {}
    Solucao(const vector<Pedido> &pedidos, int multa) : pedidos(pedidos), multa(multa) {}

    void calcularMulta(const vector<vector<int>> &s) {
        multa = 0;
        int tempo_atual = 0;

        for (int i = 0; i < pedidos.size(); i++) {
            if (i > 0) {
                tempo_atual += s[pedidos[i - 1].id][pedidos[i].id];
            }
            tempo_atual += pedidos[i].tempo_producao;

            if (tempo_atual > pedidos[i].prazo) {
                int atraso = tempo_atual - pedidos[i].prazo;
                multa += atraso * pedidos[i].multa;
            }
        }
    }
};


#endif // CONFIG_H