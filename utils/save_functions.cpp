void BuscaLocal(Solucao& solucao){
    std::vector<int> metodos = {0,1,2,3};
    std::vector<int> melhoriasPorK = {0, 0, 0};
    bool melhorou = false;

    while(!metodos.empty()){
        int n = rand() % metodos.size();
        switch (metodos[n]){ 
            case 0:
                {
                // Colocando dentro de um bloco de escopo
                int totalMelhorias = melhoriasPorK[0] + melhoriasPorK[1] + melhoriasPorK[2];
                int k = 1;
                if (totalMelhorias > 0) {
                    // escolher k baseado na frequência de melhorias anteriores
                    int sorteio = rand() % totalMelhorias;
                    if (sorteio < melhoriasPorK[0]) {
                        k = 1;
                    } else if (sorteio < melhoriasPorK[0] + melhoriasPorK[1]) {
                        k = 2;
                    } else {
                        k = 3;
                    }
                }
                else {
                    // se não houver melhorias ainda, escolhe k aleatoriamente
                    k = 1 + rand() % 3;
                }
                melhorou = bestImprovementSwapK(solucao,k);
                if(melhorou) {
                    melhoriasPorK[k-1]++;
                }
                break;
            }
            case 1:
                melhorou = bestImprovementReinsertion(solucao);
                break;
            case 2:
                melhorou = bestImprovement2opt(solucao);
                break;
            case 3:
                melhorou = bestImprovementSwap(solucao);
                break;
        }

        if(melhorou){
            metodos = {0,1,2,3};
        } else {
            metodos.erase(metodos.begin() + n);
        }
    }
}