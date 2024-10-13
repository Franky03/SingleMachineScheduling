#include "reader.h" 
#include "solver.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <filesystem> 
#include <unordered_map>
#include <numeric>
#include <algorithm>

namespace fs = std::filesystem;

double calcularGapMain(double optimal, double found){
    if (optimal == 0) return 0;
    return ((found - optimal) / optimal) * 100;
}

std::pair<double, double> rodarAlgoritmo(Solucao& solucao, const Setup& setup, const std::string& instanceName){
    auto start = std::chrono::high_resolution_clock::now();

    ILS_Opt(solucao, setup, instanceName);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    return {solucao.multaSolucao, elapsed_seconds.count()};
}

void rodarExperimento(Solucao &solucaoOriginal, const double valorOtimo, int numExecucoes, const std::string& instanceName, const Setup& setup){
    std::vector<double> resultadosMulta;
    std::vector<double> temposExecucao;

    for (int i = 0; i < numExecucoes; i++){
        std::cout << "Running " << i+1 << "/" << numExecucoes << " Times." << std::endl;
        Solucao solucao = solucaoOriginal;
        auto [multa, tempo] = rodarAlgoritmo(solucao, setup, instanceName);
        resultadosMulta.push_back(multa);
        temposExecucao.push_back(tempo);
    }

    double mediaMulta = std::accumulate(resultadosMulta.begin(), resultadosMulta.end(), 0.0) / numExecucoes;
    double mediaTempo = std::accumulate(temposExecucao.begin(), temposExecucao.end(), 0.0) / numExecucoes;

    double melhorMulta = *std::min_element(resultadosMulta.begin(), resultadosMulta.end());

    double gap = calcularGapMain(valorOtimo, melhorMulta);

    std::string nomeArquivo = "../results/" + instanceName + "_resultados.txt";
    std::ofstream arquivoResultado(nomeArquivo);

    if (arquivoResultado.is_open()) {
        arquivoResultado << "Resultados para a instância: " << instanceName << "\n\n";
        arquivoResultado << "Número de execuções: " << numExecucoes << "\n";
        arquivoResultado << "Valor ótimo: " << valorOtimo << "\n";
        arquivoResultado << "Média da multa: " << mediaMulta << "\n";
        arquivoResultado << "Melhor multa: " << melhorMulta << "\n";
        arquivoResultado << "Média do tempo de execução: " << mediaTempo << " segundos\n";
        arquivoResultado << "Gap em relação ao valor ótimo: " << gap << "\n";

        arquivoResultado.close();
        std::cout << "Resultados salvos em: " << nomeArquivo << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo de resultados: " << nomeArquivo << std::endl;
    }

}

void rodarBuscaLocalApenas(Solucao &solucaoOriginal, const Setup& setup, const std::string& instanceName){
    std::vector<double> resultadosMulta;
    std::vector<double> temposExecucao;

    for(int i = 0; i < 10; i++){
        Solucao nova = *Construcao(&solucaoOriginal, setup, 0.5);
        nova.calcularMulta(setup);
        auto start = std::chrono::high_resolution_clock::now();
        BuscaLocal(nova, setup);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        resultadosMulta.push_back(nova.multaSolucao);
        temposExecucao.push_back(elapsed_seconds.count());
    }

    double mediaMulta = std::accumulate(resultadosMulta.begin(), resultadosMulta.end(), 0.0) / 10;
    double mediaTempo = std::accumulate(temposExecucao.begin(), temposExecucao.end(), 0.0) / 10;

    double melhorMulta = *std::min_element(resultadosMulta.begin(), resultadosMulta.end());

    double gap = calcularGapMain(setup.valorOtimo, melhorMulta);    

    std::string nomeArquivo = "../rvnd/" + instanceName + "_resultados.txt";
    std::ofstream arquivoResultado(nomeArquivo);

    if (arquivoResultado.is_open()) {
        
        arquivoResultado << "Resultados para a instância: " << instanceName << "\n\n";
        arquivoResultado << "Média do tempo de execução: " << mediaTempo << " segundos\n";
        arquivoResultado << std::fixed << std::setprecision(2);
        arquivoResultado << "Gap em relação ao valor ótimo: " << gap << "\n";
        arquivoResultado << "Número de execuções: " << 10 << "\n";
        arquivoResultado << std::fixed << std::setprecision(0);
        arquivoResultado << "Valor ótimo: " << setup.valorOtimo << "\n";
        arquivoResultado << "Média da multa: " << mediaMulta << "\n";
        arquivoResultado << "Melhor multa: " << melhorMulta << "\n";
        

        arquivoResultado.close();
        std::cout << "Resultados salvos em: " << nomeArquivo << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo de resultados: " << nomeArquivo << std::endl;
    }
}

void rodarConstrucaoApenas(Solucao &solucaoOriginal, const Setup& setup, const std::string& instanceName){
    std::vector<double> resultadosMulta;
    std::vector<double> temposExecucao;

    for(int i = 0; i < 10; i++){
        auto start = std::chrono::high_resolution_clock::now();
        Solucao nova = *Construcao(&solucaoOriginal, setup, 0.5);
        nova.calcularMulta(setup);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        resultadosMulta.push_back(nova.multaSolucao);
        temposExecucao.push_back(elapsed_seconds.count());
    }

    double mediaMulta = std::accumulate(resultadosMulta.begin(), resultadosMulta.end(), 0.0) / 10;
    double mediaTempo = std::accumulate(temposExecucao.begin(), temposExecucao.end(), 0.0) / 10;

    double melhorMulta = *std::min_element(resultadosMulta.begin(), resultadosMulta.end());

    double gap = calcularGapMain(setup.valorOtimo, melhorMulta);    

    std::string nomeArquivo = "../greedy/" + instanceName + "_construcao.txt";
    std::ofstream arquivoResultado(nomeArquivo);

    if (arquivoResultado.is_open()) {
        
        arquivoResultado << "Resultados para a instância: " << instanceName << "\n\n";
        arquivoResultado << "Média do tempo de execução: " << mediaTempo << " segundos\n";
        arquivoResultado << std::fixed << std::setprecision(2);
        arquivoResultado << "Gap em relação ao valor ótimo: " << gap << "\n";
        arquivoResultado << "Número de execuções: " << 10 << "\n";
        arquivoResultado << std::fixed << std::setprecision(0);
        arquivoResultado << "Valor ótimo: " << setup.valorOtimo << "\n";
        arquivoResultado << "Média da multa: " << mediaMulta << "\n";
        arquivoResultado << "Melhor multa: " << melhorMulta << "\n";
        

        arquivoResultado.close();
        std::cout << "Resultados salvos em: " << nomeArquivo << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo de resultados: " << nomeArquivo << std::endl;
    }
}

int main(){
    
    int num_pedidos;
    Solucao solucao;
    Setup setup;

    std::unordered_map<std::string, double> optimal_values;
    std::string key;
    double value;
    std::ifstream optimalValuesFile("../data/optimal_values.txt");

    while(optimalValuesFile >> key >> value){
        optimal_values[key] = value;
    }

    optimalValuesFile.close();

    std::string instancesPath = "../instances/";

    int numExecucoes = 1;

    for(const auto& entry : fs::directory_iterator(instancesPath)){
        std::string instancePath = instancesPath + entry.path().filename().string();
        std::string instanceName = entry.path().stem().string(); // nome do arquivo sem a extensão
        readInstance(instancePath, num_pedidos, solucao.pedidos, setup.s);

        std::cout << "\033[95mInstância: " << instanceName << "\033[0m" << std::endl;
        double valorOtimo;
        if(optimal_values[instanceName])  valorOtimo = optimal_values[instanceName];
        else {
            valorOtimo = 0;
        }
        setup.valorOtimo = valorOtimo;

        rodarExperimento(solucao, valorOtimo, numExecucoes, instanceName, setup);
    }

    

    return 0;
    
}
