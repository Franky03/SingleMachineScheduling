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
    bool operator<(const Pedido &p) const {
        return prazo < p.prazo;
    }
};

struct Solucao {
    vector<Pedido> pedidos;
    vector<int> tempoAcumulado;
    vector<int> multaPorPedido;
    int multaSolucao = 0;

    Solucao() : multaSolucao(0) {}
    Solucao(const vector<Pedido> &pedidos, int multa) : pedidos(pedidos), multaSolucao(multa) {}

    void atualizarPedido(int indice, const vector<vector<int>> &s){
        if(indice == 0){
            tempoAcumulado[indice] = s[0][pedidos[indice].id] + pedidos[indice].tempo_producao;
        } else {
            tempoAcumulado[indice] = tempoAcumulado[indice - 1] + s[pedidos[indice - 1].id][pedidos[indice].id] + pedidos[indice].tempo_producao;
        }

        if(tempoAcumulado[indice] > pedidos[indice].prazo){
            int atrazo = tempoAcumulado[indice] - pedidos[indice].prazo;    
            multaPorPedido[indice] = atrazo * pedidos[indice].multa;
        } else {
            multaPorPedido[indice] = 0;
        }
    }

    void calcularMultaOpt(const vector<vector<int>> &s){
        multaSolucao = 0;
        for (int i = 0; i < pedidos.size(); ++i) {
            atualizarPedido(i, s);
            multaSolucao += multaPorPedido[i];
        }
    }

    void multaParaTrocas(int i, int j, const vector<vector<int>> &s){
        if (i > j) std::swap(i, j);
        std::swap(pedidos[i], pedidos[j]);

        multaSolucao -= multaPorPedido[i] + multaPorPedido[j];

        if (i > 0) atualizarPedido(i - 1, s);
        atualizarPedido(i, s);
        if (j > 0 && j != i + 1) atualizarPedido(j - 1, s);
        atualizarPedido(j, s);

        multaSolucao += multaPorPedido[i] + multaPorPedido[j];
    }

    double multaParaTrocasTeste(int i, int j, const vector<vector<int>> &s){
        double multa = multaSolucao;
        if (i > j) std::swap(i, j);
        std::swap(pedidos[i], pedidos[j]);

        multa -= multaPorPedido[i] + multaPorPedido[j];

        if (i > 0) atualizarPedido(i - 1, s);
        atualizarPedido(i, s);
        if (j > 0 && j != i + 1) atualizarPedido(j - 1, s);
        atualizarPedido(j, s);
        
        multa += multaPorPedido[i] + multaPorPedido[j];
       
        return multa;
    }

    void calcularMulta(const vector<vector<int>> &s) {
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