#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <utility>  // para std::pair
#include <functional>  // para std::hash

using namespace std;

struct Pedido {
    int tempo_producao;
    int prazo;
    int multa;
    int id;
    double multaPedido;
};


struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class Setup {
public:
    vector<vector<int>> s;

    Setup() {}
    
    Setup(int numPedidos) {
        s.resize(numPedidos, vector<int>(numPedidos + 1, 0));
    }

    // obtém o valor de setup entre dois pedidos
    int obterSetup(int id1, int id2) const {
        return s[id1+1][id2];
    }
};

class Solucao {
public:
    vector<Pedido> pedidos;
    int multaSolucao = 0;
    vector<int> tempoAcumulado;
    vector<double> multaPedidos;

    Solucao() : multaSolucao(0) {}
    Solucao(const vector<Pedido>& pedidos, int multa) : pedidos(pedidos), multaSolucao(multa) {}

    // Calcula a multa de um pedido específico
    double calcularMultaPedido(int tempoAtual, const Pedido& pedido) {
        if (tempoAtual > pedido.prazo) {
            int atraso = tempoAtual - pedido.prazo;
            return atraso * pedido.multa;
        }
        return 0;
    }

    // Calcula a multa total usando o objeto de Setup
    void calcularMulta(const Setup& setup) {
        multaSolucao = 0;
        int tempo_atual = setup.obterSetup(0, pedidos[0].id) + pedidos[0].tempo_producao;

        tempoAcumulado = vector<int>(pedidos.size(), 0);
        multaPedidos = vector<double>(pedidos.size(), 0);

        tempoAcumulado[0] = tempo_atual;
        multaPedidos[0] = calcularMultaPedido(tempo_atual, pedidos[0]);
        multaSolucao += multaPedidos[0];

        for (int i = 1; i < pedidos.size(); ++i) {
            tempo_atual += setup.obterSetup(pedidos[i - 1].id, pedidos[i].id) + pedidos[i].tempo_producao;
            tempoAcumulado[i] = tempo_atual;
            double m = calcularMultaPedido(tempo_atual, pedidos[i]);
            multaPedidos[i] = multaPedidos[i - 1] + m;
            multaSolucao += m;
        }
    }  

    double calcularMultaEval(vector<Pedido> &pedidos, const Setup& setup) {
        double multa = 0;
        int tempo_atual = setup.obterSetup(0, pedidos[0].id) + pedidos[0].tempo_producao;

        multa += calcularMultaPedido(tempo_atual, pedidos[0]);

        for (int i = 1; i < pedidos.size(); ++i) {
            tempo_atual += setup.obterSetup(pedidos[i - 1].id, pedidos[i].id) + pedidos[i].tempo_producao;
            multa += calcularMultaPedido(tempo_atual, pedidos[i]);
        }

        return multa;
    }
};


#endif // CONFIG_H