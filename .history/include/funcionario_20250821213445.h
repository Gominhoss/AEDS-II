/**
 * @file funcionario.h
 * @brief Definições de tipos e protótipos de funções para manipulação de dados de funcionários.
 *
 * Este arquivo contém a definição da struct TFunc e os protótipos das funções
 * para gerenciar funcionários, incluindo criação, salvamento, leitura, ordenação e busca.
 */

#ifndef FUNCIONARIOS_FUNCIONARIO_H
#define FUNCIONARIOS_FUNCIONARIO_H

#define FUNCIONARIOS_FILE "funcionarios.dat"

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

/**
 * @brief Estrutura para representar um funcionário.
 */
typedef struct Funcionario {
    int cod;                    /**< Código único do funcionário. */
    char nome[50];              /**< Nome do funcionário. */
    char cpf[15];               /**< CPF do funcionário. */
    char data_nascimento[11];   /**< Data de nascimento do funcionário. */
    double salario;             /**< Salário do funcionário. */
} TFunc;

/**
 * @brief Imprime os dados de um funcionário.
 * @param func Ponteiro para a struct TFunc a ser impressa.
 */
void imprime_funcionario(TFunc *func);

/**
 * @brief Cria uma nova estrutura TFunc em memória.
 * @param cod Código do funcionário.
 * @param nome Nome do funcionário.
 * @param cpf CPF do funcionário.
 * @param data_nascimento Data de nascimento do funcionário.
 * @param salario Salário do funcionário.
 * @return Ponteiro para a struct TFunc criada.
 */
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);

/**
 * @brief Salva os dados de um funcionário em um arquivo.
 * @param func Ponteiro para a struct TFunc a ser salva.
 * @param out Ponteiro para o arquivo de saída.
 */
void salva_funcionario(TFunc *func, FILE *out);

/**
 * @brief Lê os dados de um funcionário de um arquivo.
 * @param in Ponteiro para o arquivo de entrada.
 * @return Ponteiro para a struct TFunc lida, ou NULL se o fim do arquivo for alcançado.
 */
TFunc *le_funcionario(FILE *in);

/**
 * @brief Lê e imprime todos os funcionários de um arquivo.
 * @param in Ponteiro para o arquivo a ser lido.
 */
void le_funcionarios(FILE *in);

/**
 * @brief Calcula o tamanho em bytes da struct TFunc.
 * @return O tamanho da struct TFunc.
 */
int tamanho_funcionario();

/**
 * @brief Calcula o número de registros de funcionários em um arquivo.
 * @param arq Ponteiro para o arquivo.
 * @return O número de registros no arquivo.
 */
int tamanho_arquivo_funcionario(FILE *arq);

/**
 * @brief Retorna o tamanho de um registro de funcionário.
 * @return O tamanho do registro de funcionário.
 */
int tamanho_registro_funcionario();

/**
 * @brief Busca um funcionário por código usando busca binária.
 * @param cod Código do funcionário a ser buscado.
 * @param arq Ponteiro para o arquivo de dados (deve estar ordenado).
 * @param tam Número de registros no arquivo.
 * @return Ponteiro para o funcionário encontrado, ou NULL se não for encontrado.
 */
TFunc* busca_binaria_funcionario(int cod, FILE *arq, int tam);

/**
 * @brief Busca um funcionário por código usando busca sequencial.
 * @param cod Código do funcionário a ser buscado.
 * @param arq Ponteiro para o arquivo de dados.
 * @return Ponteiro para o funcionário encontrado, ou NULL se não for encontrado.
 */
TFunc* busca_sequencial_funcionario(int cod, FILE *arq);

/**
 * @brief Ordena um arquivo de funcionários usando o método Insertion Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void insertion_sort_disco_funcionario(FILE *arq, int tam);

/**
 * @brief Gera uma base de dados desordenada de funcionários.
 * @param file Ponteiro para o arquivo onde a base será gerada.
 * @param numberRecords Número de registros a serem gerados.
 */
void gerarBaseDesordenada_funcionario(FILE* file, int numberRecords);

/**
 * @brief Ordena um arquivo de funcionários usando o método Selection Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void selection_sort_disco_funcionario(FILE *arq, int tam);

/**
 * @brief Embaralha os registros em um arquivo de funcionários.
 * @param arq Ponteiro para o arquivo a ser embaralhado.
 * @param total_records Número total de registros no arquivo.
 */
void embaralhar_arquivo_funcionario(FILE *arq, int total_records);

#endif //FUNCIONARIOS_FUNCIONARIO_H
