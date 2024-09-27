#include "reader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void readInstance(const string& filename, int& num_pedidos, vector<Pedido>& pedidos, vector<vector<int>>& s){
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
    s.resize(num_pedidos + 1, vector<int>(num_pedidos, 0));

    // Lê a matriz de setup
    for (int i = 0; i < num_pedidos + 1; ++i) {
        for (int j = 0; j < num_pedidos; ++j) {
            file >> s[i][j];  // Preenche o vetor de vetores com os tempos de setup
        }
    }

    file.close();
}