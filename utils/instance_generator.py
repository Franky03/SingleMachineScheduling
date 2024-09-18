import random
import numpy as np
def gerar_instancia_precisa(N):
    # Gerar listas de tempo de preparo aleatórias
    tempo_preparo = [random.randint(1, 20) for _ in range(N)]
    
    # Gerar uma matriz simétrica NxN com valores entre 1 e 5
    matriz = np.zeros((N, N), dtype=int)
    for i in range(N):
        for j in range(i, N):
            if i != j:
                valor = random.randint(1, 5)
                matriz[i][j] = valor
                matriz[j][i] = valor

    # Escolher uma ordem aleatória para garantir que seja sem multas
    ordem_aleatoria = list(range(N))
    random.shuffle(ordem_aleatoria)
    
    # Gerar prazos com base na soma do tempo de preparo e setups para a ordem aleatória
    prazos = [0] * N
    total_time = 0

    for i in range(N):
        pedido_atual = ordem_aleatoria[i]
        total_time += tempo_preparo[pedido_atual]
        if i > 0:
            pedido_anterior = ordem_aleatoria[i-1]
            total_time += matriz[pedido_anterior][pedido_atual]  # Setup entre o pedido anterior e o atual
        prazos[pedido_atual] = total_time + random.randint(1, 5)  # Garantir um prazo maior do que o tempo acumulado
    
    # Gerar multas aleatórias entre 10 e 100
    multas = [random.randint(10, 100) for _ in range(N)]


    # Escrever para um arquivo
    nome_arquivo = f'../data/instancia_precisa_{N}.txt'
    with open(nome_arquivo, 'w') as f:
        f.write(f'{N}' + '\n')
        f.write(' '.join(map(str, tempo_preparo)) + '\n')
        f.write(' '.join(map(str, prazos)) + '\n')
        f.write(' '.join(map(str, multas)) + '\n')
        f.write('\n')
        
        for linha in matriz:
            f.write(' '.join(map(str, linha)) + '\n')

    print(f"Arquivo '{nome_arquivo}' gerado com sucesso!")

# Testar com N = 100
N = 200
gerar_instancia_precisa(N)