#include "solver.h"
#include <cmath>
#include <cstdlib>  // Para rand()
#include <ctime>    // Para time()
#include <fstream>  // Para escrever em arquivos

#define MAX_ITER 100
#define MAX_ITER_ILS 200

void BuscaLocal(Solucao& solucao, std::vector<std::vector<int>>& s){
    std::vector<int> metodos = {0,1,2};
    bool melhorou = false;

    while(!metodos.empty()){
        int n = rand() % metodos.size();
        switch (metodos[n]){
            case 0:
                //std::cout<< "Swap Movement\n" << std::endl;
                melhorou = bestImprovementSwap(solucao, s);
                break;
            case 1:
                //std::cout<< "Insert Movement\n" << std::endl;
                melhorou = bestImprovementInsert(solucao, s);
                break;
            case 2:
                //std::cout<< "2-opt Movement\n" << std::endl;
                melhorou = bestImprovement2opt(solucao, s);
                break;
        }

        if(melhorou){
            metodos = {0,1,2};
        } else {
            metodos.erase(metodos.begin() + n);
        }
    }
}

void DoubleBridge(Solucao &solucao){
    int n = solucao.pedidos.size();

    int pos1 = 1 + rand() % (n / 4);
    int pos2 = pos1 + 1 + rand() % (n / 4);
    int pos3 = pos2 + 1 + rand() % (n / 4);
    
    std::vector<Pedido> novoVetor;
    novoVetor.insert(novoVetor.end(), solucao.pedidos.begin(), solucao.pedidos.begin() + pos1); // A
    novoVetor.insert(novoVetor.end(), solucao.pedidos.begin() + pos3, solucao.pedidos.end()); // D
    novoVetor.insert(novoVetor.end(), solucao.pedidos.begin() + pos2, solucao.pedidos.begin() + pos3); // C
    novoVetor.insert(novoVetor.end(), solucao.pedidos.begin() + pos1, solucao.pedidos.begin() + pos2); // B

    solucao.pedidos = novoVetor;
}

void Perturbar(Solucao &solucao){
    // exemplo: fazer um movimento aleatório de troca de dois pedidos
    int i = rand() % solucao.pedidos.size();
    int j = rand() % solucao.pedidos.size();

    // garantir que i e j não sejam iguais
    while (i == j) {
        j = rand() % solucao.pedidos.size();
    }

    std::swap(solucao.pedidos[i], solucao.pedidos[j]);
}

void ILS(Solucao &solucao, std::vector<std::vector<int>>& s){
    Solucao melhorSolucao;
    for(int i=0; i < MAX_ITER; ++i){
        Solucao novaSolucao = *Construcao(&solucao, s, 0.1);
        melhorSolucao = novaSolucao;

        int iterILS = 0;
        while(iterILS < MAX_ITER_ILS){
            //std::cout << "Iteração " << i << " do ILS, iteração " << iterILS << " da busca local" << std::endl;
            BuscaLocal(novaSolucao, s);

            // Critério de aceitação: aceita se for melhor
            if (novaSolucao.multaSolucao < melhorSolucao.multaSolucao) {
                melhorSolucao = novaSolucao;
                iterILS = 0;
            }

            DoubleBridge(novaSolucao);
            iterILS++;
        }

         if (novaSolucao.multaSolucao < melhorSolucao.multaSolucao) {
            melhorSolucao = novaSolucao;
        }

    }

    solucao = melhorSolucao;
}

void SimulatedAnnealing(Solucao &solucao, std::vector<std::vector<int>>& s){

    srand(time(0));

    Solucao atualSolucao = *Construcao(&solucao, s, 0.1);
    Solucao melhorSolucao = atualSolucao;

    double temperaturaInicial = 10000;
    double temperaturaFinal = 0.001;
    double alpha = 0.9;
    double temperatura = temperaturaInicial;
    int iter = 0;

    std::ofstream outputFile("../data/simulated_annealing.csv");
    outputFile << "Iteracao,Temperatura,Custo\n";  // Cabeçalho do arquivo CSV

    while (temperatura > temperaturaFinal) {
        Solucao novaSolucao = atualSolucao;

        BuscaLocal(novaSolucao, s);
        
        double deltaMulta = novaSolucao.multaSolucao - atualSolucao.multaSolucao;

        if (deltaMulta < 0 || std::exp(-deltaMulta / temperatura) > ((double) rand() / (RAND_MAX))) {
            atualSolucao = novaSolucao;
        } 

        if (atualSolucao.multaSolucao < melhorSolucao.multaSolucao) {
            melhorSolucao = atualSolucao;
        }

        outputFile << iter << "," << temperatura << "," << atualSolucao.multaSolucao << "\n";

        temperatura *= alpha;
        DoubleBridge(atualSolucao);  // Perturbação da solução atual
        iter++;
    }

    outputFile.close();

    solucao = melhorSolucao;

}