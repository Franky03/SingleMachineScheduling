#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <chrono>
#include <cstdlib>  // Para std::rand e std::srand
#include <ctime>    // Para std::time

using namespace std;
using namespace std::chrono;

void generateRandomNumbers(vector<int> &vec, list<int> &lst, int size) {
    for (int i = 0; i < size; ++i) {
        int num = std::rand() % 1000; // Números aleatórios entre 0 e 999
        vec.push_back(num);
        lst.push_back(num);
    }
}

int main() {
    const int SIZE = 10000;
    const int REPEATS = 10000;

    // Inicializar gerador de números aleatórios
    std::srand(std::time(0));

    // Variáveis para somar os tempos
    long long total_vec = 0;
    long long total_lst = 0;

    for (int i = 0; i < REPEATS; ++i) {
        // Criar vetor e lista
        vector<int> vec;
        list<int> lst;

        // Gerar números aleatórios
        generateRandomNumbers(vec, lst, SIZE);

        // Medir o tempo das operações básicas para vector
        auto start = high_resolution_clock::now();

        // Operações básicas para vector
        vec.size();
        vec.front();
        vec.back();
        vec.push_back(1000);
        vec.pop_back();
        vector<int>::iterator it_vec = vec.begin();
        while (it_vec != vec.end()) ++it_vec;

        auto end = high_resolution_clock::now();
        total_vec += duration_cast<microseconds>(end - start).count();

        // Medir o tempo das operações básicas para list
        start = high_resolution_clock::now();

        // Operações básicas para list
        lst.size();
        lst.front();
        lst.back();
        lst.push_back(1000);
        lst.pop_back();
        list<int>::iterator it_lst = lst.begin();
        while (it_lst != lst.end()) ++it_lst;

        end = high_resolution_clock::now();
        total_lst += duration_cast<microseconds>(end - start).count();
    }

    // Calcular tempos médios
    double average_vec = static_cast<double>(total_vec) / REPEATS;
    double average_lst = static_cast<double>(total_lst) / REPEATS;

    // Salvar os resultados em um arquivo
    ofstream file("tempo_operacoes.txt");
    if (file.is_open()) {
        file << "Tempo médio para operações básicas em vector (microsegundos): " << average_vec << endl;
        file << "Tempo médio para operações básicas em list (microsegundos): " << average_lst << endl;
        file.close();
        cout << "Tempos médios salvos em 'tempo_operacoes.txt'" << endl;
    } else {
        cout << "Não foi possível abrir o arquivo para escrita." << endl;
    }

    return 0;
}
