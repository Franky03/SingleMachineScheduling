#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Função para remover tabs e espaços duplos de uma linha
string cleanLine(const string& line) {
    string cleanedLine;
    bool spaceFound = false;

    for (char c : line) {
        if (isspace(c)) {
            if (!spaceFound) {
                cleanedLine += ' ';
                spaceFound = true;
            }
        } else {
            cleanedLine += c;
            spaceFound = false;
        }
    }

    // Remover espaço no final da linha, se houver
    if (!cleanedLine.empty() && cleanedLine.back() == ' ') {
        cleanedLine.pop_back();
    }

    return cleanedLine;
}

// Função para processar o arquivo e padronizá-lo
void padronizarInstancia(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada: " << inputFile << endl;
        return;
    }

    if (!outFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de saída: " << outputFile << endl;
        return;
    }

    string line;
    bool firstLine = true;  // Usado para controlar se estamos na primeira linha

    while (getline(inFile, line)) {
        string cleanedLine = cleanLine(line);

        // Se não for a primeira linha, adicione uma quebra de linha antes
        if (!firstLine && !cleanedLine.empty()) {
            outFile << "\n";
        }
        
        if (!cleanedLine.empty()) {
            outFile << cleanedLine;
        }

        firstLine = false;  // Depois da primeira linha, desativa essa flag
    }

    inFile.close();
    outFile.close();

    cout << "Instância padronizada salva em: " << outputFile << endl;
}

int main() {
    string folderPath = "../instances";  // Coloque o caminho da pasta com os arquivos

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            string inputFile = entry.path().string();
            string outputFile = folderPath + entry.path().filename().string();

            padronizarInstancia(inputFile, outputFile);
        }
    }

    return 0;
}