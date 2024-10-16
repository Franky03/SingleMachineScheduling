# Abrir o arquivo com os números separados por espaço
with open("lista_inicial.txt", "r") as f:
    # Separar os números por espaço
    lista = f.read().split()
    # Converter cada item para inteiro
    lista = [int(x) for x in lista]

# analisar se a lista vai ter os numeros de 0 a 199
print(set(lista) == set(range(400)))

# Criar uma nova lista com cada número incrementado em 1
lista_final = [num + 1 for num in lista]

# Confirmar se a lista final é igual à lista inicial + 1
print(lista_final == [x + 1 for x in lista])

# Salvar a lista final em um arquivo, separando os números por vírgula
with open('lista_final.txt', 'w') as f:
    f.write(','.join(map(str, lista_final)))
