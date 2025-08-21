/**
 * @file fornecedor.h
 * @brief Definições de tipos e protótipos de funções para manipulação de dados de fornecedores.
 *
 * Este arquivo contém a definição da struct TForn e os protótipos das funções
 * para gerenciar fornecedores, incluindo criação, salvamento, leitura, ordenação e busca.
 */

#ifndef FORNECEDOR_H_INCLUDED
#define FORNECEDOR_H_INCLUDED

#define FORNECEDOR_FILE "fornecedores.dat"

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/**
 * @brief Estrutura para representar um fornecedor.
 */
typedef struct Fornecedor {
  int cod;        /**< Código único do fornecedor. */
  char nome[50];  /**< Nome do fornecedor. */
  char cnpj[19];  /**< CNPJ do fornecedor. */
} TForn;

/**
 * @brief Imprime os dados de um fornecedor.
 * @param forn Ponteiro para a struct TForn a ser impressa.
 */
void imprime_fornecedor(TForn *forn);

/**
 * @brief Cria uma nova estrutura TForn em memória.
 * @param cod Código do fornecedor.
 * @param nome Nome do fornecedor.
 * @param cnpj CNPJ do fornecedor.
 * @return Ponteiro para a struct TForn criada.
 */
TForn *fornecedor(int cod, char *nome, char *cnpj);

/**
 * @brief Salva os dados de um fornecedor em um arquivo.
 * @param forn Ponteiro para a struct TForn a ser salva.
 * @param out Ponteiro para o arquivo de saída.
 */
void salva_fornecedor(TForn *forn, FILE *out);

/**
 * @brief Lê os dados de um fornecedor de um arquivo.
 * @param in Ponteiro para o arquivo de entrada.
 * @return Ponteiro para a struct TForn lida, ou NULL se o fim do arquivo for alcançado.
 */
TForn *le_fornecedor(FILE *in);

/**
 * @brief Lê e imprime todos os fornecedores de um arquivo.
 * @param in Ponteiro para o arquivo a ser lido.
 */
void le_fornecedores(FILE *in);

/**
 * @brief Calcula o tamanho em bytes da struct TForn.
 * @return O tamanho da struct TForn.
 */
int tamanho_fornecedor();

/**
 * @brief Calcula o número de registros de fornecedores em um arquivo.
 * @param arq Ponteiro para o arquivo.
 * @return O número de registros no arquivo.
 */
int tamanho_arquivo_fornecedor(FILE *arq);

/**
 * @brief Retorna o tamanho de um registro de fornecedor.
 * @return O tamanho do registro de fornecedor.
 */
int tamanho_registro_fornecedor();

/**
 * @brief Gera uma base de dados desordenada de fornecedores.
 * @param file Ponteiro para o arquivo onde a base será gerada.
 * @param numberRecords Número de registros a serem gerados.
 */
void gerarBaseDesordenada_fornecedor(FILE *file, int numberRecords);

/**
 * @brief Busca um fornecedor por código usando busca sequencial.
 * @param cod Código do fornecedor a ser buscado.
 * @param arq Ponteiro para o arquivo de dados.
 * @return A struct TForn encontrada.
 */
TForn busca_sequencial_fornecedor(int cod, FILE *arq);

/**
 * @brief Busca um fornecedor por código usando busca binária.
 * @param cod Código do fornecedor a ser buscado.
 * @param arq Ponteiro para o arquivo de dados (deve estar ordenado).
 * @param tam Número de registros no arquivo.
 * @return A struct TForn encontrada.
 */
TForn busca_binaria_fornecedor(int cod, FILE *arq, int tam);

/**
 * @brief Ordena um arquivo de fornecedores usando o método Insertion Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void insertion_sort_disco_fornecedor(FILE *arq, int tam);

/**
 * @brief Ordena um arquivo de fornecedores usando o método Selection Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void selection_sort_disco_fornecedor(FILE *arq, int tam);

/**
 * @brief Embaralha os registros em um arquivo de fornecedores.
 * @param arq Ponteiro para o arquivo a ser embaralhado.
 * @param total_records Número total de registros no arquivo.
 */
void embaralhar_arquivo_fornecedor(FILE *arq, int total_records);

#endif // FORNECEDOR_H_INCLUDED