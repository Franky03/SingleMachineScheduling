#include "solver.h"
#include <cmath>
#include <cstdlib>  // Para rand()
#include <ctime>    // Para time()
#include <fstream>  // Para escrever em arquivos
#include <thread>
#include <mutex>

#define MAX_ITER 100
#define MAX_ITER_ILS 200
#define L 200
#define NUM_THREADS 4

std::mutex mtx;
std::mutex stopMtx;
bool stop = false;

void BuscaLocal(Solucao& solucao, std::vector<std::vector<int>>& s){
    std::vector<int> metodos = {0,1,2};
    bool melhorou = false;

    while(!metodos.empty()){
        int n = rand() % metodos.size();
        switch (metodos[n]){ 
            // switch case é mais eficiente que if-else encadeado para múltiplas comparações pois é implementado como uma tabela de saltos
            // enquanto o if-else no pior caso, tem complexidade O(n)
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

void MarkovChains(int thread_id, int L_start, int L_end, Solucao &atualSolucao, Solucao &melhorSolucao, double temperatura, std::vector<std::vector<int>>& s, std::ofstream& outputFile, int& iter){
    for (int i = L_start; i < L_end; ++i){

        {
            std::lock_guard<std::mutex> guard(stopMtx);
            if (stop) return;  // se a flag estiver ativada, interrompe a execução
        }

        Solucao novaSolucao = atualSolucao;
        BuscaLocal(novaSolucao, s);
        double deltaMulta = novaSolucao.multaSolucao - atualSolucao.multaSolucao;

        {
            std::lock_guard<std::mutex> guard(mtx);
            if (deltaMulta < 0 || std::exp(-deltaMulta / temperatura) > ((double) rand() / (RAND_MAX))) {
                atualSolucao = novaSolucao;
            }
            if (atualSolucao.multaSolucao < melhorSolucao.multaSolucao) {
                melhorSolucao = atualSolucao;
            }

            outputFile << iter << "," << temperatura << "," << melhorSolucao.multaSolucao << "\n";
            iter++;

            if (melhorSolucao.multaSolucao == 0) {
                std::lock_guard<std::mutex> stopGuard(stopMtx);
                stop = true;  // Define a flag para parar todas as threads
            }
        }

        {
            std::lock_guard<std::mutex> guard(stopMtx);
            if (stop) return;  // Para se a flag for ativada
        }
    }
}

void SimulatedAnnealing(Solucao &solucao, std::vector<std::vector<int>>& s) {
    srand(time(0));

    Solucao atualSolucao = *gulosao(&solucao, s);
    Solucao melhorSolucao = atualSolucao;

    double temperaturaInicial = 1e5;
    double temperaturaFinal = 1e-3;
    double alpha = 0.92;
    double temperatura = temperaturaInicial;
    int iter = 0;
    

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
            std::lock_guard<std::mutex> guard(stopMtx);
            if (stop) break;  // se a flag estiver ativada, interrompe o loop
        }

        // reduzir a temperatura após gerar a cadeia de Markov de tamanho L
        temperatura *= alpha;

        // aplicar a perturbação na solução atual após a cadeia de Markov
        DoubleBridge(atualSolucao);
    }

    outputFile.close();

    solucao = melhorSolucao;
}