# Projeto de Gerenciamento de Supermercado em C

Este é um projeto desenvolvido para a disciplina de Algoritmos e Estruturas de Dados II (AEDS-II). O seu principal objetivo é demonstrar a manipulação de grandes volumes de dados utilizando majoritariamente a memória secundária (disco), simulando uma aplicação real onde os dados não caberiam completamente na memória RAM. Todas as operações, como ordenação e busca, são implementadas em C para interagir diretamente com arquivos binários.

## Funcionalidades

- **Criação de Bases de Dados**: Gera arquivos de dados (`.dat`) com registros desordenados para cada entidade (funcionários, clientes, etc.).
- **Ordenação em Disco**: Implementa o algoritmo **Selection Sort** para ordenar os arquivos de dados diretamente no disco, sem carregar todo o conteúdo para a memória.
- **Busca**: Realiza buscas por código nos arquivos de dados usando **Busca Sequencial** e **Busca Binária** (requer arquivo ordenado).
- **Log de Performance**: Mede e registra o tempo de execução das principais operações em `log/log_execucao.txt`.

## Estrutura do Projeto

O projeto está organizado da seguinte forma:

```
/AEDS-II
|-- src/                # Contém todos os arquivos de código-fonte (.c)
|-- include/            # Contém todos os arquivos de cabeçalho (.h)
|-- bin/                # Onde o executável final (main.exe) é gerado
|-- data/               # Diretórios para armazenar arquivos de partição (se usado)
|-- log/                # Contém o arquivo de log de execução
|-- .gitignore          # Arquivos e pastas ignorados pelo Git
|-- build.bat           # Script para compilar o projeto no Windows
|-- README.md           # Este arquivo
```

## Como Compilar e Executar

### Pré-requisitos

- Um compilador C, como o **GCC** (por exemplo, via [MinGW-w64](https://www.mingw-w64.org/) no Windows).

### Compilação

Para compilar o projeto, basta executar o script `build.bat` na raiz do diretório:

```bash
.\build.bat
```

Isso irá compilar todos os arquivos da pasta `src` e gerar o executável `main.exe` dentro da pasta `bin`.

### Execução

Após a compilação, navegue até a pasta `bin` e execute o programa:

```bash
cd bin
.\main.exe
```
