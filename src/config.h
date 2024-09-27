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
    unordered_map<pair<int, int>, int, hash_pair> s;

    Setup() {}

    // adiciona uma transição de setup no mapa
    void adicionarSetup(int id1, int id2, int valor) {
        if (valor != 0) {
            s[{id1, id2}] = valor;
        }
    }

    // pbtém o valor de setup entre dois pedidos
    int obterSetup(int id1, int id2) const {
        auto it = s.find({id1, id2});
        if (it != s.end()) {
            return it->second;
        }
        return 0;  // retorna 0 se não houver transição
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

            multaPedidos[i] = calcularMultaPedido(tempo_atual, pedidos[i]);
            multaSolucao += multaPedidos[i];
        }
    }    
};


#endif // CONFIG_H