#include "solver.h"
#include <cmath>
#include <cstdlib>  // Para rand()
#include <ctime>    // Para time()
#include <fstream>  // Para escrever em arquivos
#include <thread>
#include <atomic>
#include <mutex>

#define MAX_ITER 150
#define MAX_ITER_ILS 200
#define L 200
#define NUM_THREADS 4

std::mutex mtx;
std::mutex stopMtx;
std::atomic<bool> stop(false);

void BuscaLocal(Solucao& solucao, std::vector<std::vector<int>>& s){
    std::vector<int> metodos = {0,1,2};
    bool melhorou = false;
    int count = 0;

    while(!metodos.empty()){
        int n = rand() % metodos.size();
        switch (metodos[n]){ 
            case 0:
                melhorou = bestImprovementSwap(solucao, s);
                if(melhorou) count++;
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

    solucao.pedidos = std::move(novoVetor); // Move novoVetor para solucao.pedidos
}

void ILS(Solucao &solucao, std::vector<std::vector<int>>& s) {
    Solucao melhorSolucao = solucao;  // Inicializa melhorSolucao como uma cópia da solução atual
    Solucao novaSolucao;

    for (int i = 0; i < MAX_ITER; ++i) {
        novaSolucao = *Construcao(&solucao, s, 0.20);  // Nova solução construída

        Solucao melhorLocal = novaSolucao;  // Copia a nova solução como a melhor solução local

        int iterILS = 0;
        while (iterILS < MAX_ITER_ILS) {
            BuscaLocal(novaSolucao, s);

            if (novaSolucao.multaSolucao < melhorLocal.multaSolucao) {
                melhorLocal = novaSolucao;  // Cópia da nova solução como a melhor local
                iterILS = 0;
            }

            DoubleBridge(novaSolucao);
            novaSolucao.calcularMulta(s);
            iterILS++;
        }

        // Verifica se a melhor solução local é melhor que a global
        if (melhorLocal.multaSolucao < melhorSolucao.multaSolucao) {
            melhorSolucao = melhorLocal;
        }
    }

    solucao = melhorSolucao;  // Atualiza a solução final
}

void MarkovChains(int thread_id, int L_start, int L_end, Solucao &atualSolucao, Solucao &melhorSolucao, double temperatura, std::vector<std::vector<int>>& s, std::ofstream& outputFile, std::atomic<int>& iter){
    for (int i = L_start; i < L_end; ++i){

        {
            if(stop.load()) return;  // para se a flag for ativada
        }

        Solucao novaSolucao = atualSolucao;
        BuscaLocal(novaSolucao, s);
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

void SimulatedAnnealing(Solucao &solucao, std::vector<std::vector<int>>& s) {
    srand(static_cast<unsigned int>(time(0)));

    Solucao atualSolucao = *gulosao(&solucao, s);
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
                    temperatura, std::ref(s), std::ref(outputFile), 
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