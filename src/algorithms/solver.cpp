#include "solver.h"
#include <cmath>
#include <cstdlib>  // Para rand()
#include <random>   // Para std::mt19937
#include <ctime>    // Para time()
#include <fstream>  // Para escrever em arquivos
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <cassert>


#define MAX_ITER 200
#define MAX_ITER_ILS 400
#define L 200
#define NUM_THREADS 5
#define MAX_ITER_SEM_MELHORA 25
#define MAX_LOCAL_SEARCH 100

std::mutex mtx;
std::atomic<bool> stop(false);

void BuscaLocal(Solucao& solucao, const Setup& setup) {
    std::vector<int> metodos = {0,1,2,3,4,5,6,7,8};
    bool melhorou = false;

    while(!metodos.empty()){
        int n = rand() % metodos.size();
        switch (metodos[n]){ 
            case 0:
                melhorou = bestImprovementSwap(solucao, setup);
                break;
            case 1:
                melhorou = bestImprovementInsert(solucao, setup);
                break;
            case 2:
                melhorou = bestImprovementShift(solucao, setup, 2);
                break;
            case 3:
                melhorou = bestImprovementShift(solucao, setup,3);
                break;
            case 4:
                melhorou = bestImprovementShift(solucao, setup, 4);
                break;
            case 5:
                melhorou = bestImprovementSwapK(solucao, setup, 2);
                break;
            case 6:
                melhorou = bestImprovementSwapK(solucao, setup, 3);
                break;
            case 7:
                melhorou = bestImprovementSwapK(solucao, setup, 4);
                break;
            case 8:
                melhorou = bestImprovement2opt(solucao, setup);
                break;
        }
        if(melhorou){   
            metodos = {0,1,2,3,4,5,6,7,8};
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
    
    // Utiliza iteradores para acessar as posições no vetor
    auto itBegin = solucao.pedidos.begin();
    auto itPos1 = itBegin + pos1;
    auto itPos2 = itBegin + pos2;
    auto itPos3 = itBegin + pos3;

    std::vector<Pedido> novoVetor;
    novoVetor.reserve(n);

    novoVetor.insert(novoVetor.end(), itBegin, itPos1); // A
    novoVetor.insert(novoVetor.end(), itPos3, solucao.pedidos.end()); // D
    novoVetor.insert(novoVetor.end(), itPos2, itPos3); // C
    novoVetor.insert(novoVetor.end(), itPos1, itPos2); // B

    solucao.pedidos = std::move(novoVetor); // move novoVetor para solucao.pedidos
}

void ILS_thread(Solucao& melhorSolucaoGlobal, int iterStart, int iterEnd, const Setup& setup) {
    Solucao melhorSolucao = melhorSolucaoGlobal;  // Inicializa melhorSolucao como uma cópia da solução global
    Solucao novaSolucao;

    for (int i = iterStart; i < iterEnd; ++i) {
        
        if (stop.load()) {
            break;
        }

        novaSolucao = *Construcao(&melhorSolucao, setup, 0.1);
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
            
            DoubleBridge(novaSolucao);
            novaSolucao.calcularMulta(setup);
        
            iterILS++;
        }

        // se a melhor solução local for melhor que a melhor solução global
        {
            std::lock_guard<std::mutex> lock(mtx);  // trava o mutex durante a comparação
            if (melhorLocal.multaSolucao < melhorSolucaoGlobal.multaSolucao) {
                melhorSolucaoGlobal = melhorLocal;
                 std::cout << "Thread " << std::this_thread::get_id() << " - Iteração " << i 
                        << " - Melhor solução local: " << melhorLocal.multaSolucao << std::endl;
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



void ILS(Solucao &solucao, const Setup &setup) {
    Solucao melhorSolucao = solucao;  // Inicializa melhorSolucao como uma cópia da solução atual
    Solucao novaSolucao;

    double temperatura = 1e5;

    for (int i = 0; i < MAX_ITER; ++i) {
        novaSolucao = *Construcao(&solucao, setup ,0.20);  // Nova solução construída

        Solucao melhorLocal = novaSolucao;  // Copia a nova solução como a melhor solução local

        int iterILS = 0;
        while (iterILS < MAX_ITER_ILS) {
            BuscaLocal(novaSolucao, setup);  

            if (novaSolucao.multaSolucao < melhorLocal.multaSolucao) {
                melhorLocal = novaSolucao;  // Cópia da nova solução como a melhor local
                iterILS = 0;
            }

            DoubleBridge(novaSolucao);
            iterILS++;
        }

        // Verifica se a melhor solução local é melhor que a global
        if (melhorLocal.multaSolucao < melhorSolucao.multaSolucao) {
            melhorSolucao = melhorLocal;
            std::cout << "Iteração " << i << " - Melhor solução local: " << melhorLocal.multaSolucao << std::endl;
        }
    }

    solucao = melhorSolucao;  // Atualiza a solução final
}

void MarkovChains(int thread_id, int L_start, int L_end, Solucao &atualSolucao, Solucao &melhorSolucao, double temperatura, std::ofstream& outputFile, std::atomic<int>& iter){
    for (int i = L_start; i < L_end; ++i){

        {
            if(stop.load()) return;  // para se a flag for ativada
        }

        Solucao novaSolucao = atualSolucao;
        //BuscaLocal(novaSolucao);
        double deltaMulta = novaSolucao.multaSolucao - atualSolucao.multaSolucao;

        {
            if (deltaMulta < 0 || std::exp(-deltaMulta / temperatura) > ((double) rand() / (RAND_MAX))) {
                std::lock_guard<std::mutex> guard(mtx);
                atualSolucao = novaSolucao;
            }
            if (atualSolucao.multaSolucao < melhorSolucao.multaSolucao) {
                std::lock_guard<std::mutex> guard(mtx);
                melhorSolucao = atualSolucao;
            }

            std::lock_guard<std::mutex> guard(mtx);
            outputFile << iter << "," << temperatura << "," << melhorSolucao.multaSolucao << "\n";
            iter++;

            if (melhorSolucao.multaSolucao == 0) {
                stop.store(true);
            }
        }

        {
            if(stop.load()) return;  // para se a flag for ativada
        }
    }
}

void SimulatedAnnealing(Solucao &solucao, const Setup &setup) {
    srand(static_cast<unsigned int>(time(0)));

    Solucao atualSolucao = *gulosao(&solucao, setup);
    Solucao melhorSolucao = atualSolucao;

    double temperaturaInicial = 1e5;
    double temperaturaFinal = 1e-3;
    double alpha = 0.92;
    double temperatura = temperaturaInicial;
    std::atomic<int> iter(0);
    

    std::ofstream outputFile("../data/simulated_annealing.csv");
    outputFile << "Iteracao,Temperatura,Custo\n"; 

    while (temperatura > temperaturaFinal) {
        std::vector<std::thread> threads;
        int chunkSize = L / NUM_THREADS;

        for (int t = 0; t < NUM_THREADS; ++t){
            int L_start = t * chunkSize;
            int L_end = (t == NUM_THREADS - 1) ? L : L_start + chunkSize;
            threads.push_back(
                std::thread(
                    MarkovChains, 
                    t, L_start, L_end, 
                    std::ref(atualSolucao), std::ref(melhorSolucao), 
                    temperatura, std::ref(outputFile), 
                    std::ref(iter)
                )
            );
        }

        for(auto& t : threads){
            t.join();
        }

        {
            if(stop.load()) break;  // para se a flag for ativada
        }

        // reduzir a temperatura após gerar a cadeia de Markov de tamanho L
        temperatura *= alpha;

        // aplicar a perturbação na solução atual após a cadeia de Markov
        DoubleBridge(atualSolucao);
    }

    outputFile.close();
    std::cout << "Encontrada na iteração " << iter << std::endl;
    solucao = melhorSolucao;
}