#include "reader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void readInstance(const string& filename, int& num_pedidos, vector<Pedido>& pedidos, unordered_map<pair<int, int>, int, hash_pair>& s){
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string line;

    // Lê o número de pedidos
    getline(file, line);
    stringstream ss(line);
    ss >> num_pedidos;

    pedidos.resize(num_pedidos);

    for(int i=0; i < num_pedidos; i++){
        pedidos[i].id = i;
    }
    
    for (int i = 0; i < num_pedidos; ++i) {
        file >> pedidos[i].tempo_producao;
    }


    for (int i = 0; i < num_pedidos; ++i) {
        file >> pedidos[i].prazo;
    }

    for (int i = 0; i < num_pedidos; ++i) {
        file >> pedidos[i].multa;
    }

    // Ignora linha vazia
    getline(file, line);

    // Lê a matriz de setup
    int setup;
    for (int i = 0; i < num_pedidos + 1; ++i) {
        for (int j = 0; j < num_pedidos; ++j) {
            file >> setup;
            if (setup != 0) {
                s[{i, j}] = setup;  // Armazena apenas os tempos de setup não-zero
            }
        }
    }

    file.close();
}