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

#endif // CONFIG_H