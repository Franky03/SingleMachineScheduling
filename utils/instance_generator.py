import random
import numpy as np

def gerar_instancia(N):
    # Gerar listas conforme as condições especificadas
    tempo_preparo = [random.randint(1, 20) for _ in range(N)]
    prazos = [random.randint(tp + 1, 20) if tp < 20 else 20 for tp in tempo_preparo]
    multas = [random.randint(10, 100) for _ in range(N)]
    
    # Gerar uma matriz simétrica NxN com valores entre 5 e 10
    matriz = np.zeros((N, N), dtype=int)
    for i in range(N):
        for j in range(i, N):
            valor = random.randint(1,5)
            matriz[i][j] = valor    
            matriz[j][i] = valor  # Garantir que a matriz seja simétrica

            if i == j:
                matriz[i][j] = 0 
    
    nome_arquivo = f'../data/instancia_{N}.txt'
    with open(nome_arquivo, 'w') as f:
        f.write(f'{N}' + '\n')
        f.write('\n')
        f.write(' '.join(map(str, tempo_preparo)) + '\n')
        f.write(' '.join(map(str, prazos)) + '\n')
        f.write(' '.join(map(str, multas)) + '\n')
        f.write('\n')

        for linha in matriz:
            f.write(' '.join(map(str, linha)) + '\n')

    print(f"Arquivo '{nome_arquivo}' gerado com sucesso!")

N = 20
gerar_instancia(N)