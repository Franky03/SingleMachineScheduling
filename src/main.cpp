#include <iostream>
#include "reader.h" 
#include "guloso.h"  
#include "neighbor.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <filesystem>

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

std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&time_now);

    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d_%H-%M-%S");  // Formato: YYYY-MM-DD_HH-MM-SS
    return oss.str();
}

int main(){
    
    int num_pedidos;
    vector<vector<int>> s;
    Solucao solucao;

    readInstance("../data/input.txt", num_pedidos, solucao.pedidos, s);
    solucao.calcularMulta(s);

    std::string timestamp = getCurrentDateTime();
    std::string logFileName = "../logs/execution_log_" + timestamp + ".txt";

    std::ofstream logFile(logFileName);
    
    if (!logFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de log" << std::endl;
        return 1;
    }

    logFile << "Multa inicial: " << solucao.multaSolucao << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    ILS(solucao, s);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;


    logFile << "Ordem final dos pedidos: ";
    for (const auto& pedido : solucao.pedidos) {
        logFile << pedido.id << " ";
    }
    logFile << std::endl;

    logFile << "Multa final: " << solucao.multaSolucao << std::endl;
    logFile << "Tempo de execução: " << elapsed_seconds.count() << " segundos" << std::endl;

    logFile.close();

    return 0;
    
}
//437868