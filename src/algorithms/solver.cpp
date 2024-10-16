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
#include <sstream>

#define MAX_ITER 600
#define MAX_ITER_ILS 600

std::mutex mtx;
std::mutex mtxResultados;  
std::atomic<bool> stop(false);

int numExecLocalSearch = 0;

double calcularGap(double optimal, double found){
    if (optimal == 0) return 0;
    return ((found - optimal) / optimal) * 100;
}

void BuscaLocal(Solucao& solucao, const Setup& setup) {
    // a complexidade da busca local vai ser a complexidade do movimento escolhido
    std::vector<int> metodos = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};
    bool melhorou = false;

    while (!metodos.empty()) {
        int n = rand() % metodos.size();
        switch (metodos[n]) {
            case 0:
                melhorou = bestImprovementSwap(solucao, setup);
                break;
            case 1:
                melhorou = bestImprovementInsert(solucao, setup);
                break;
            case 2:
                melhorou = bestImprovementReinsert(solucao, setup, 2);
                break;
            case 3:
                melhorou = bestImprovementReinsert(solucao, setup, 3);
                break;
            case 4:
                melhorou = bestImprovementReinsert(solucao, setup, 4);
                break;
            case 5:
                melhorou = bestImprovementReinsert(solucao, setup, 5);
                break;
            case 6:
                melhorou = bestImprovementReinsert(solucao, setup, 6);
                break;
            case 7:
                melhorou = bestImprovementReinsert(solucao, setup, 7);
                break;
            case 8:
                melhorou = bestImprovementReinsert(solucao, setup, 8);
                break;
            case 9:
                melhorou = bestImprovementReinsert(solucao, setup, 9);
                break;
            case 10:
                melhorou = bestImprovementReinsert(solucao, setup, 10);
                break;
            case 11:
                melhorou = bestImprovementReinsert(solucao, setup, 11);
                break;
            case 12:
                melhorou = bestImprovementReinsert(solucao, setup, 12);
                break;
            case 13:
                melhorou = bestImprovementReinsert(solucao, setup, 13);
                break;
            case 14:
                melhorou = bestImprovementReinsert(solucao, setup, 14);
                break;
            case 15:
                melhorou = bestImprovementReinsert(solucao, setup, 15);
                break;
            case 16:
                melhorou = bestImprovementReinsert(solucao, setup, 16);
                break;
            case 17:
                melhorou = bestImprovementReinsert(solucao, setup, 17);
                break;
            case 18:
                melhorou = bestImprovementReinsert(solucao, setup, 18);
                break;
            case 19:
                melhorou = bestImprovementReinsert(solucao, setup, 19);
                break;
            case 20:
                melhorou = bestImprovementReinsert(solucao, setup, 20);
                break;
            case 21:
                melhorou = bestImprovementReinsert(solucao, setup, 21);
                break;
            case 22:
                melhorou = bestImprovementReinsert(solucao, setup, 22);
                break;
            case 23:
                melhorou = bestImprovementReinsert(solucao, setup, 23);
                break;
            case 24:
                melhorou = bestImprovementReinsert(solucao, setup, 24);
                break;
            case 25:
                melhorou = bestImprovement2opt(solucao, setup);
                break;
            case 26:
                melhorou = bestImprovementSwapK(solucao, setup, 2);
                break;
            case 27:
                melhorou = bestImprovementSwapK(solucao, setup, 3);
                break;
            case 28:
                melhorou = bestImprovementSwapK(solucao, setup, 4);
                break;
            case 29:
                melhorou = bestImprovementSwapK(solucao, setup, 5);
                break;
            case 30:
                melhorou = bestImprovementSwapK(solucao, setup, 6);
                break;
            case 31:
                melhorou = bestImprovementSwapK(solucao, setup, 7);
                break;
            case 32:
                melhorou = bestImprovementSwapK(solucao, setup, 8);
                break;
            case 33:
                melhorou = bestImprovementSwapK(solucao, setup, 9);
                break;
            case 34:
                melhorou = bestImprovementSwapK(solucao, setup, 10);
                break;
            case 35:
                melhorou = bestImprovementSwapK(solucao, setup, 2);
                break;
            case 36:
                melhorou = bestImprovementSwapK(solucao, setup, 3);
                break;
            case 37:
                melhorou = bestImprovementSwapK(solucao, setup, 4);
                break;
            case 38:
                melhorou = bestImprovementSwapK(solucao, setup, 5);
                break;
            case 39:
                melhorou = bestImprovementSwapK(solucao, setup, 6);
                break;
        }

        if (melhorou) {
            // Reseta os métodos para tentar novamente
            metodos = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};
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

void SmoothPertubation(Solucao &solucao){
    int n = solucao.pedidos.size();
    int pos1 = rand() % n;
    int pos2 = rand() % n;

    std::swap(solucao.pedidos[pos1], solucao.pedidos[pos2]);
}

void FineTuning(Solucao& solucao, const Setup& setup, const std::string& nomeArquivo) {
    std::vector<int> idsPedidos;
    
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return;
    }

    std::string linha;

    if (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        int pedidoId;

        while (ss >> pedidoId) {
            idsPedidos.push_back(pedidoId); 
        }
    }

    arquivo.close();

    std::vector<Pedido> pedidosReordenados;
    std::unordered_map<int, Pedido> pedidosMap;
    for (const auto& pedido : solucao.pedidos) {
        pedidosMap[pedido.id] = pedido;
    }

    for(const auto& id : idsPedidos){
        auto it = pedidosMap.find(id); 
        if (it != pedidosMap.end()) {
            pedidosReordenados.push_back(it->second);
        }
        else {
            std::cerr << "Pedido não encontrado: " << id << std::endl;
        }
    }

    solucao.pedidos = pedidosReordenados;
    solucao.calcularMulta(setup);
}

void ILS_thread(Solucao& melhorSolucaoGlobal, int iterStart, int iterEnd, const Setup& setup) {
    Solucao melhorSolucao = melhorSolucaoGlobal;  // inicializa melhorSolucao como uma cópia da solução global
    Solucao novaSolucao;

    for (int i = iterStart; i < iterEnd; ++i) {
        
        if (stop.load()) {
            break;
        }

        FineTuning(melhorSolucao, setup, "../fine_tuning/n200_ft.txt");
        novaSolucao = melhorSolucao;
        Solucao melhorLocal = novaSolucao; 

        int iterILS = 0;
        while (iterILS < MAX_ITER_ILS) {    
            BuscaLocal(novaSolucao, setup);
            if (novaSolucao.multaSolucao < melhorLocal.multaSolucao) {
                melhorLocal = novaSolucao;  
                std::cout << "Thread " << std::this_thread::get_id() << " - Iteração " << i 
                << " - Melhor solução local: \033[32m" << melhorLocal.multaSolucao << "\033[0m" << std::endl;
                
                if (melhorLocal.multaSolucao < melhorSolucaoGlobal.multaSolucao) {
                     std::string nomeArquivo = "../resultados_copa_apa/resultados.txt";
                        std::ofstream arquivoResultado(nomeArquivo);

                        if (arquivoResultado.is_open()) {
                            arquivoResultado << "Melhor solução encontrada: " << melhorLocal.multaSolucao << "\n";
                            for (int i = 0; i < melhorLocal.pedidos.size(); ++i) {
                                arquivoResultado << melhorLocal.pedidos[i].id << " ";
                            }

                            arquivoResultado.close();
                        } else {
                            std::cerr << "Erro ao abrir o arquivo de resultados: " << nomeArquivo << std::endl;
                        }

                        melhorSolucaoGlobal = melhorLocal;
                }


                iterILS = 0;
            }

            if (melhorLocal.multaSolucao == 0) {
                stop.store(true);
                break;
            }
            
            novaSolucao = melhorLocal;
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

                std::string nomeArquivo = "../resultados_copa_apa/resultados.txt";
                std::ofstream arquivoResultado(nomeArquivo);

                if (arquivoResultado.is_open()) {
                    arquivoResultado << "Melhor solução encontrada: " << melhorLocal.multaSolucao << "\n";
                    for (int i = 0; i < melhorLocal.pedidos.size(); ++i) {
                        arquivoResultado << melhorLocal.pedidos[i].id << " ";
                    }

                    arquivoResultado.close();
                } else {
                    std::cerr << "Erro ao abrir o arquivo de resultados: " << nomeArquivo << std::endl;
                }


            }
           
        }

        if (melhorSolucaoGlobal.multaSolucao == 0) {
            stop.store(true); 
            break;  
        }
    }
}

void ILS_Opt(Solucao& solucao, const Setup& setup, const std::string& instanceName) {
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
