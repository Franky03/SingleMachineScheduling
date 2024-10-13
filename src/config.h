#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <utility> 
#include <functional> 

using namespace std;

struct Pedido {
    int tempo_producao;
    int prazo;
    int multa;
    int id;
};

class Setup {
public:
    vector<vector<int>> s;
    double valorOtimo;

    Setup() {}
    
    Setup(int numPedidos) {
        s.resize(numPedidos, vector<int>(numPedidos + 1, 0));
    }

    int obterSetup(int id1, int id2) const {
        return s[id1+1][id2];
    }

    int obterSetupPrimeiroPedido(int id) const {
        return s[0][id];
    }
};

class Solucao {
public:
    vector<Pedido> pedidos;
    int multaSolucao = 0;
    vector<int> tempoAcumulado;
    vector<double> multaAcumulada;

    Solucao() : multaSolucao(0) {}
    Solucao(const vector<Pedido>& pedidos, int multa) : pedidos(pedidos), multaSolucao(multa) {}

    // calcula a multa de um pedido
    inline double calcularMultaPedido(int tempoAtual, const Pedido& pedido) {
        if (tempoAtual > pedido.prazo) {
            int atraso = tempoAtual - pedido.prazo;
            return atraso * pedido.multa;
        }
        return 0;
    }

    void calcularMulta(const Setup& setup) {
        multaSolucao = 0;
        int tempo_atual = setup.obterSetupPrimeiroPedido(pedidos[0].id) + pedidos[0].tempo_producao;

        tempoAcumulado = vector<int>(pedidos.size(), 0);
        multaAcumulada = vector<double>(pedidos.size(), 0);

        tempoAcumulado[0] = tempo_atual;
        multaAcumulada[0] = calcularMultaPedido(tempo_atual, pedidos[0]);
        multaSolucao += multaAcumulada[0];

        for (int i = 1; i < pedidos.size(); ++i) {
            tempo_atual += setup.obterSetup(pedidos[i - 1].id, pedidos[i].id) + pedidos[i].tempo_producao;
            tempoAcumulado[i] = tempo_atual;
            double m = calcularMultaPedido(tempo_atual, pedidos[i]);
            multaAcumulada[i] = multaAcumulada[i - 1] + m;
            multaSolucao += m;
        }
    }  

    double calcularMultaEval(vector<Pedido> &pedidos, const Setup& setup) {
        double multa = 0;
        int tempo_atual = setup.obterSetupPrimeiroPedido(pedidos[0].id) + pedidos[0].tempo_producao;

        multa += calcularMultaPedido(tempo_atual, pedidos[0]);

        for (int i = 1; i < pedidos.size(); ++i) {
            tempo_atual += setup.obterSetup(pedidos[i - 1].id, pedidos[i].id) + pedidos[i].tempo_producao;
            multa += calcularMultaPedido(tempo_atual, pedidos[i]);
        }

        return multa;
    }
};


#endif 