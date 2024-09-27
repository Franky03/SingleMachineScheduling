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
    vector<int> tempoAcumulado;
    vector<double> multaPedidos;

    Solucao() : multaSolucao(0) {}
    Solucao(const vector<Pedido> &pedidos, int multa, vector<vector<int>> setup) : pedidos(pedidos), multaSolucao(multa), s(setup){}

    void calcularMulta() {
        multaSolucao = 0;
        int tempo_atual = 0;

        tempoAcumulado = vector<int>(pedidos.size(), 0);
        multaPedidos = vector<double>(pedidos.size(), 0);

        tempo_atual += s[0][pedidos[0].id];
        tempo_atual += pedidos[0].tempo_producao;
        tempoAcumulado[0] = tempo_atual;

        if (tempo_atual > pedidos[0].prazo) {
            int atraso = tempo_atual - pedidos[0].prazo;
            multaSolucao += atraso * pedidos[0].multa;
            multaPedidos[0] = multaSolucao;
        }

        for (int i = 1; i < pedidos.size(); i++) {
            // adiciona o tempo de setup entre o pedido anterior e o atual
            tempo_atual += s[pedidos[i - 1].id][pedidos[i].id];
            tempo_atual += pedidos[i].tempo_producao;
            tempoAcumulado[i] = tempo_atual;

            if (tempo_atual > pedidos[i].prazo) {
                int atraso = tempo_atual - pedidos[i].prazo;
                multaSolucao += atraso * pedidos[i].multa;
                multaPedidos[i] = multaSolucao;
            }
        }
    }    


    double atualizarMulta(int posicaoInicio){
        double multaAtual = multaSolucao;
        double novaMulta = 0;
        int tempo_atual = 0;

        if(posicaoInicio == 0){
            tempo_atual += s[0][pedidos[0].id];
            tempo_atual += pedidos[0].tempo_producao;

            if (tempo_atual > pedidos[0].prazo) {
                int atraso = tempo_atual - pedidos[0].prazo;
                novaMulta += atraso * pedidos[0].multa;
            }
        }

        for (size_t i = posicaoInicio; i < pedidos.size() - 1; ++i) {
            // adiciona o tempo de setup entre o pedido anterior e o atual
            tempo_atual += s[pedidos[i - 1].id][pedidos[i].id];
            tempo_atual += pedidos[i].tempo_producao;

            if (tempo_atual > pedidos[i].prazo) {
                int atraso = tempo_atual - pedidos[i].prazo;
                novaMulta += atraso * pedidos[i].multa;
            }
        }

        multaAtual -= calcularMultaParcial(posicaoInicio);
        multaAtual += novaMulta;

        return multaAtual;
    }

    double calcularMultaParcial(int posicaoInicio) const {
        double multaParcial = 0;

        int tempo_atual = 0;

        if(posicaoInicio == 0){
            tempo_atual += s[0][pedidos[0].id];
            tempo_atual += pedidos[0].tempo_producao;

            if (tempo_atual > pedidos[0].prazo) {
                int atraso = tempo_atual - pedidos[0].prazo;
                multaParcial += atraso * pedidos[0].multa;
            }
        }

        for (size_t i = posicaoInicio; i < pedidos.size() - 1; ++i) {
            tempo_atual += s[pedidos[i - 1].id][pedidos[i].id];
            tempo_atual += pedidos[i].tempo_producao;

            if (tempo_atual > pedidos[i].prazo) {
                int atraso = tempo_atual - pedidos[i].prazo;
                multaParcial += atraso * pedidos[i].multa;
            }
        }
        return multaParcial;
    }
    
};


#endif // CONFIG_H