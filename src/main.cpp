#include <iostream>
#include "reader.h" 
#include "guloso.h"  

using namespace std;

int calcularMulta(const vector<Pedido>& pedidos, const vector<vector<int>>& s) {
    int tempo_atual = 0;
    int soma_multa = 0;

    for (int i = 0; i < pedidos.size(); i++){
        if (i > 0){
            tempo_atual += s[pedidos[i-1].id][pedidos[i].id];
        }
        tempo_atual += pedidos[i].tempo_producao;

        if (tempo_atual> pedidos[i].prazo){
            int atraso = tempo_atual - pedidos[i].prazo;
            soma_multa += atraso * pedidos[i].multa;
        }
    }

    return soma_multa;
}

int main(){
    int num_pedidos;
    vector<Pedido> pedidos;
    vector<vector<int>> s;

    readInstance("../data/input.txt", num_pedidos, pedidos, s);

   // printar a ordem dos pedidos (pedido id)
    for (int i = 0; i < num_pedidos; i++) {
        cout << pedidos[i].id << " ";
    }
    cout << endl;

    vector<Pedido> pedidosOrdenados = gulosao(pedidos, s);

    for (int i = 0; i < num_pedidos; i++) {
        cout << pedidosOrdenados[i].id << " ";
    }
    cout << endl;
    
    cout << calcularMulta(pedidosOrdenados, s) << endl;

    return 0;
    
}