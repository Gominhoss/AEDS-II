# Projeto de Gerenciamento de Supermercado em C

Este é um projeto desenvolvido para a disciplina de Algoritmos e Estruturas de Dados II (AEDS-II). O seu principal objetivo é demonstrar a manipulação de grandes volumes de dados utilizando majoritariamente a memória secundária (disco), simulando uma aplicação real onde os dados não caberiam completamente na memória RAM. Todas as operações, como ordenação e busca, são implementadas em C para interagir diretamente com ficheiros binários.

## Funcionalidades

  - **Criação de Bases de Dados**: Gera ficheiros de dados (`.dat`) com registos desordenados para cada entidade (funcionários, clientes, etc.).
  - **Tabela Hash com Encadeamento Exterior**: Para a entidade **Funcionário**, foi implementada uma tabela hash de alta performance com tratamento de colisões por encadeamento exterior. Isto permite operações de busca, inserção e remoção com complexidade média de O(1).
  - **Busca Eficiente**:
      - **Busca por Hash**: Acesso direto e rápido aos registos de funcionários.
      - **Busca Sequencial e Binária**: Métodos de busca tradicionais disponíveis para as outras entidades do sistema (a busca binária requer um ficheiro ordenado).
  - **Ordenação em Disco**: Implementa o algoritmo **Selection Sort** para ordenar os ficheiros de dados diretamente no disco, sem carregar todo o conteúdo para a memória.
  - **Log de Performance**: Mede e regista o tempo de execução das principais operações em `log/log_execucao.txt`.

## Estrutura do Projeto

O projeto está organizado da seguinte forma:

```
/AEDS-II
|-- src/                # Contém todos os ficheiros de código-fonte (.c)
|-- include/            # Contém todos os ficheiros de cabeçalho (.h)
|-- bin/                # Onde o executável final (main.exe) é gerado
|-- data/               # Diretórios para armazenar ficheiros de partição (se usado)
|-- log/                # Contém o ficheiro de log de execução
|-- .gitignore          # Ficheiros e pastas ignorados pelo Git
|-- build.bat           # Script para compilar o projeto no Windows
|-- README.md           # Este ficheiro
```

## Como Compilar e Executar

### Pré-requisitos

  - Um compilador C, como o **GCC** (por exemplo, via [MinGW-w64](https://www.mingw-w64.org/) no Windows).

### Compilação

Para compilar o projeto, basta executar o script `build.bat` na raiz do diretório:

```bash
.\build.bat
```

Isto irá compilar todos os ficheiros da pasta `src` e gerar o executável `main.exe` dentro da pasta `bin`.

### Execução

Após a compilação, navegue até a pasta `bin` e execute o programa:

```bash
cd bin
.\main.exe
```
