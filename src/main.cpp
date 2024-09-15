#include "reader.h" 
#include "solver.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <filesystem>


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

    SimulatedAnnealing(solucao, s);
    //ILS(solucao, s);

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
