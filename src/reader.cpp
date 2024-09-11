#include "reader.h"
#include <iostream>
#include <fstream>

using namespace std;

void readInstance(const string& filename, int& num_pedidos, vector<Pedido>& pedidos, vector<vector<int>>& s){
    ifstream file(filename);

    if (!file.is_open()){
        cout << "Erro ao abrir o arquivo " << filename << endl;
        exit(1);
    }

    file >> num_pedidos;
    pedidos.resize(num_pedidos);
    s.resize(num_pedidos, vector<int>(num_pedidos));

    for (int i = 0; i < num_pedidos; i++) {
        pedidos[i].id = i; // Leitura dos ids
    }

    for (int i = 0; i < num_pedidos; i++) {
        file >> pedidos[i].tempo_producao; // Leitura dos tempos de produção
    }
    for (int i = 0; i < num_pedidos; i++) {
        file >> pedidos[i].prazo; // Leitura dos prazos
    }
    for (int i = 0; i < num_pedidos; i++) {
        file >> pedidos[i].multa; // Leitura das multas
    }
    for (int i = 0; i < num_pedidos; i++) {
        for (int j = 0; j < num_pedidos; j++) {
            file >> s[i][j]; // Leitura da matriz de adjacência
        }
    }
    file.close();

    

}