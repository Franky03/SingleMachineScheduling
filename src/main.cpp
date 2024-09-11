#include <iostream>
#include "reader.h" 
#include "guloso.h"  

using namespace std;

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
    
    return 0;
    
}