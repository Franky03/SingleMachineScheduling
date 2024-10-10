#include "solver.h"
#include <cmath>
#include <cstdlib> 
#include <random>  
#include <ctime>
#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <cassert>


#define MAX_ITER 100
#define MAX_ITER_ILS 200
#define L 200
#define NUM_THREADS 5
#define MAX_ITER_SEM_MELHORA 25
#define MAX_LOCAL_SEARCH 100

std::mutex mtx;
std::atomic<bool> stop(false);

void BuscaLocal(Solucao& solucao, const Setup& setup) { 
    // a complexidade da busca local vai ser a complexidade do movimento escolhido
    std::vector<int> metodos = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    bool melhorou = false;
    int count = 0;

    while(!metodos.empty()){
        int n = rand() % metodos.size();
        switch (metodos[n]){
            case 0:
                melhorou = bestImprovementSwap(solucao, setup);
                break;
            case 1:
                melhorou = bestImprovementReinsertion(solucao, setup,1);
                break;
            case 2:
                melhorou = bestImprovementReinsertion(solucao, setup, 2);
                break;
            case 3:
                melhorou = bestImprovementReinsertion(solucao, setup,3);
                break;
            case 4:
                melhorou = bestImprovementReinsertion(solucao, setup, 4);
                break;
            case 5:
                melhorou = bestImprovementSwapK(solucao, setup, 2);
                break;
            case 6:
                melhorou = bestImprovementSwapK(solucao, setup, 6);
                break;
            case 7:
                melhorou = bestImprovementReinsertion(solucao, setup, 10);
                break;
            case 8:
                melhorou = bestImprovementReinsertion(solucao, setup, 12);
                break;
            case 9:
                melhorou = bestImprovementReinsertion(solucao, setup, 14);
                break;
            case 10:
                melhorou = bestImprovementReinsertion(solucao, setup, 8);
                break;
            case 11:
                melhorou = bestImprovementReinsertion(solucao, setup, 6);
                break;
            case 12:
                melhorou = bestImprovementReinsertion(solucao, setup, 5);
                break;
            case 13:
                melhorou = bestImprovementReinsertion(solucao, setup, 7);
                break;
            case 14:
                melhorou = bestImprovementReinsertion(solucao, setup, 9);
                break;
            case 15:
                melhorou = bestImprovementReinsertion(solucao, setup, 11);
                break;
            case 16:
                melhorou = bestImprovementReinsertion(solucao, setup, 13);
                break;
        }
        if(melhorou){
            metodos = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        } else {
            metodos.erase(metodos.begin() + n);
        }
    }
}


void EmbaralhaPedidos(Solucao &solucao){
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(solucao.pedidos.begin(), solucao.pedidos.end(), g);
}

void DoubleBridge(Solucao &solucao){
    int n = solucao.pedidos.size();

    int pos1 = 1 + rand() % (n / 4);
    int pos2 = pos1 + 1 + rand() % (n / 4);
    int pos3 = pos2 + 1 + rand() % (n / 4);
    
    auto itBegin = solucao.pedidos.begin();
    auto itPos1 = itBegin + pos1;
    auto itPos2 = itBegin + pos2;
    auto itPos3 = itBegin + pos3;

    std::vector<Pedido> novoVetor;
    novoVetor.reserve(n); // O(n)

    //insert -> linear on the number of elements inserted
    novoVetor.insert(novoVetor.end(), itBegin, itPos1); // A
    novoVetor.insert(novoVetor.end(), itPos3, solucao.pedidos.end()); // D
    novoVetor.insert(novoVetor.end(), itPos2, itPos3); // C
    novoVetor.insert(novoVetor.end(), itPos1, itPos2); // B

    solucao.pedidos = std::move(novoVetor); // move novoVetor para solucao.pedidos O(1)
}

void ILS_thread(Solucao& melhorSolucaoGlobal, int iterStart, int iterEnd, const Setup& setup) {
    Solucao melhorSolucao = melhorSolucaoGlobal;  // inicializa melhorSolucao como uma cópia da solução global
    Solucao novaSolucao;

    for (int i = iterStart; i < iterEnd; ++i) {
        
        if (stop.load()) {
            break;
        }

        novaSolucao = *Construcao(&melhorSolucao, setup, 0.8);
        Solucao melhorLocal = novaSolucao; 

        int iterILS = 0;
        while (iterILS < MAX_ITER_ILS) {    
            BuscaLocal(novaSolucao, setup);

            if (novaSolucao.multaSolucao < melhorLocal.multaSolucao) {
                melhorLocal = novaSolucao;  
                iterILS = 0;
            }

            if (melhorLocal.multaSolucao == 0) {
                stop.store(true);
                break;
            }
            
            DoubleBridge(novaSolucao); // O(n)
            novaSolucao.calcularMulta(setup);
        
            iterILS++;
        }

        // se a melhor solução local for melhor que a melhor solução global
        {
            std::lock_guard<std::mutex> lock(mtx);  // trava o mutex durante a comparação
            if (melhorLocal.multaSolucao < melhorSolucaoGlobal.multaSolucao) {
                melhorSolucaoGlobal = melhorLocal;
                 std::cout << "Thread " << std::this_thread::get_id() << " - Iteração " << i 
                << " - Melhor solução local: \033[32m" << melhorLocal.multaSolucao << "\033[0m" << std::endl;
            }
           
        }

        if (melhorSolucaoGlobal.multaSolucao == 0) {
            stop.store(true); 
            break;  
        }
    }
}

void ILS_Opt(Solucao& solucao, const Setup& setup) {
    const int numThreads = std::thread::hardware_concurrency();
    int iterPerThread = MAX_ITER / numThreads;

    solucao.calcularMulta(setup);

    Solucao melhorSolucao = solucao;

    stop.store(false);

    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; ++t) {
        int iterStart = t * iterPerThread;
        int iterEnd = (t == numThreads - 1) ? MAX_ITER : iterStart + iterPerThread;
        threads.emplace_back(ILS_thread, std::ref(melhorSolucao), iterStart, iterEnd, std::ref(setup));
    }

    for (auto& th : threads) {
        th.join();
    }

    solucao = melhorSolucao;
}