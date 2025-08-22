/**
 * @file funcionario.h
 * @brief Definições de tipos e protótipos de funções para manipulação de dados de funcionários.
 *
 * Este arquivo contém a definição da struct TFunc e os protótipos das funções
 * para gerenciar funcionários, incluindo criação, salvamento, leitura, ordenação, busca e Tabela Hash.
 */

#ifndef FUNCIONARIOS_FUNCIONARIO_H
#define FUNCIONARIOS_FUNCIONARIO_H

// ==================================================================
// ARQUIVOS E CONSTANTES
// ==================================================================
#define FUNCIONARIOS_DAT_FILE "funcionarios.dat"
#define HASH_FILE "hash_funcionarios.dat"
#define TAMANHO_TABELA_HASH 101

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

// ==================================================================
// ESTRUTURA DE DADOS
// ==================================================================
// Estrutura para representar um funcionário.
typedef struct Funcionario {
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
    
    // CAMPOS NOVOS PARA A TABELA HASH
    int ocupado; // Flag para remoção lógica (1 = ativo, 0 = removido)
    int prox;    // Posição do próximo registro na lista de colisão (-1 se for o último)

} TFunc;

// ==================================================================
// FUNÇÕES BÁSICAS DE MANIPULAÇÃO
// ==================================================================

// Imprime os dados de um funcionário.
void imprime_funcionario(TFunc *func);

// Cria uma nova estrutura TFunc em memória.
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);

// Salva os dados de um funcionário em um arquivo.
void salva_funcionario(TFunc *func, FILE *out);

// Lê os dados de um funcionário de um arquivo.
TFunc *le_funcionario(FILE *in);

// Lê e imprime todos os funcionários ativos de um arquivo.
void le_funcionarios(FILE *in);

// Retorna o tamanho de um registro de funcionário em bytes.
int tamanho_registro_funcionario();

// Calcula o número de registros de funcionários em um arquivo.
int tamanho_arquivo_funcionario(FILE *arq);

// ==================================================================
// FUNÇÕES DE GERAÇÃO, ORDENAÇÃO E BUSCA (LEGADO)
// ==================================================================

// Gera uma base de dados desordenada de funcionários.
void gerarBaseDesordenada_funcionario(FILE* file, int numberRecords);

// Embaralha os registros em um arquivo de funcionários.
void embaralhar_arquivo_funcionario(FILE *arq, int total_records);

// Busca um funcionário por código usando busca sequencial.
TFunc* busca_sequencial_funcionario(int cod, FILE *arq);

// Busca um funcionário por código usando busca binária (requer arquivo ordenado).
TFunc* busca_binaria_funcionario(int cod, FILE *arq, int tam);

// Ordena um arquivo de funcionários usando o método Selection Sort em disco.
void selection_sort_disco_funcionario(FILE *arq, int tam);

// Ordena um arquivo de funcionários usando o método Insertion Sort em disco.
void insertion_sort_disco_funcionario(FILE *arq, int tam);

// ==================================================================
// FUNÇÕES DA TABELA HASH
// ==================================================================

// Constrói a tabela hash a partir do arquivo de dados existente.
void construir_hash_da_base_existente(FILE *tabela_hash, FILE *arq_funcionarios);

// Busca um funcionário usando a Tabela Hash.
TFunc* buscar_funcionario_hash(int cod, FILE *tabela_hash, FILE *arq_funcionarios);

// Remove um funcionário usando a Tabela Hash (remoção lógica).
void remover_funcionario_hash(int cod, FILE *tabela_hash, FILE *arq_funcionarios);

// Insere um novo funcionário (com dados do usuário) no sistema.
void inserir_novo_funcionario_hash(FILE *tabela_hash, FILE *arq_funcionarios);

// Imprime a estrutura completa da Tabela Hash para depuração.
void imprimir_tabela_hash_completa(FILE *tabela_hash, FILE *arq_funcionarios);


#endif //FUNCIONARIOS_FUNCIONARIO_H