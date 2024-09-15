import matplotlib.pyplot as plt
import pandas as pd

# Ler o arquivo CSV
data = pd.read_csv('../data/simulated_annealing.csv')
fig, ax1 = plt.subplots(figsize=(10, 5))

# Plotar a Temperatura no eixo y primário
ax1.plot(data['Iteracao'], data['Temperatura'], label='Temperatura', color='r')
ax1.set_xlabel('Iteração')
ax1.set_ylabel('Temperatura', color='r')
ax1.tick_params(axis='y', labelcolor='r')

# Criar o segundo eixo y (para o Custo), compartilhando o eixo x
ax2 = ax1.twinx()
ax2.plot(data['Iteracao'], data['Custo'], label='Custo', color='b')
ax2.set_ylabel('Custo', color='b')
ax2.tick_params(axis='y', labelcolor='b')

# Título e grade
plt.title('Simulated Annealing: Temperatura e Custo ao longo das Iterações')
ax1.grid(True)

# Salvar o gráfico
plt.show()
