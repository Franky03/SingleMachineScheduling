import os

def listar_arquivos(diretorio, prefixo=''):
    conteudo = []
    itens = sorted(os.listdir(diretorio))
    for index, item in enumerate(itens):
        caminho = os.path.join(diretorio, item)
        # se for um .py ou "estrutura.txt", não exibe
        if item.endswith('.py') or item == 'estrutura.txt':
            continue
        if os.path.isdir(caminho):
            # se a pasta for .git, não exibe
            if item == '.git':
                continue
            conteudo.append(f"{prefixo}├── /{item}")
            sub_prefixo = '│   ' if index != len(itens) - 1 else '    '
            conteudo.extend(listar_arquivos(caminho, prefixo + sub_prefixo))
        else:
            conteudo.append(f"{prefixo}├── {item}")
    return conteudo

def salvar_estrutura(diretorio, arquivo_saida):
    estrutura = listar_arquivos(diretorio)
    with open(arquivo_saida, 'w') as f:
        f.write(f"/{os.path.basename(diretorio)}\n")
        f.write("│\n")
        f.write("\n".join(estrutura))

diretorio_pai = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
diretorio_avo = os.path.dirname(diretorio_pai)
data_path = os.path.join(diretorio_avo, 'data')
file_to_save = os.path.join(data_path, 'estrutura.txt')
salvar_estrutura(diretorio_avo, file_to_save)