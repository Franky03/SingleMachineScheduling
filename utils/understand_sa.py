import matplotlib.pyplot as plt
import numpy as np

# cria um gráfico para visualizar a formula
# exp(- multa / temperatura)
# multa vai ser um valor de 300000 a 0
# temperatua vai ser um valor de 1e5 a 1e-5

multa = np.linspace(0, 300000, 100)
temperatura = np.linspace(1e5, 1e-5, 100)  # Use um número inteiro aqui

# criar o exponencial
# Use a média das temperaturas para calcular a fórmula para visualização
formula = np.exp(-multa[:, np.newaxis] / temperatura)  # Broadcasting para calcular para cada temperatura

# Criação do gráfico
for i in range(formula.shape[1]):  # Plota uma linha para cada temperatura
    plt.plot(multa, formula[:, i], label=f'Temperatura: {temperatura[i]:.2e}')

plt.xlabel('Multa')
plt.ylabel('Exp(-Multa / Temperatura)')
plt.title('Visualização da Fórmula Exponencial')
plt.legend()
plt.ylim(0, 1)  # Limita o eixo Y para melhor visualização
plt.savefig('formula.png')
