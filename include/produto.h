/**
 * @file produto.h
 * @brief Definições de tipos e protótipos de funções para manipulação de dados de produtos.
 *
 * Este arquivo contém a definição da struct TProd e os protótipos das funções
 * para gerenciar produtos, incluindo criação, salvamento, leitura, ordenação e busca.
 */

#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED

#define PRODUTOS_FILE "produtos.dat"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Estrutura para representar um produto.
 */
typedef struct Produto {
  int cod;        /**< Código único do produto. */
  char nome[50];  /**< Nome do produto. */
  char valor[50]; /**< Valor do produto (armazenado como string). */
  int cod_forn;   /**< Código do fornecedor do produto. */
} TProd;

/**
 * @brief Imprime os dados de um produto.
 * @param prod Ponteiro para a struct TProd a ser impressa.
 */
void imprime_produto(TProd *prod);

/**
 * @brief Cria uma nova estrutura TProd em memória.
 * @param cod Código do produto.
 * @param nome Nome do produto.
 * @param valor Valor do produto.
 * @param cod_forn Código do fornecedor.
 * @return Ponteiro para a struct TProd criada.
 */
TProd *produto(int cod, char *nome, char *valor, int cod_forn);

/**
 * @brief Salva os dados de um produto em um arquivo.
 * @param prod Ponteiro para a struct TProd a ser salva.
 * @param out Ponteiro para o arquivo de saída.
 */
void salva_produto(TProd *prod, FILE *out);

/**
 * @brief Lê os dados de um produto de um arquivo.
 * @param in Ponteiro para o arquivo de entrada.
 * @return Ponteiro para a struct TProd lida, ou NULL se o fim do arquivo for alcançado.
 */
TProd *le_produto(FILE *in);

/**
 * @brief Lê e imprime todos os produtos de um arquivo.
 * @param in Ponteiro para o arquivo a ser lido.
 */
void le_produtos(FILE *in);

/**
 * @brief Calcula o tamanho em bytes da struct TProd.
 * @return O tamanho da struct TProd.
 */
int tamanho_produto();

/**
 * @brief Calcula o número de registros de produtos em um arquivo.
 * @param arq Ponteiro para o arquivo.
 * @return O número de registros no arquivo.
 */
int tamanho_arquivo_produto(FILE *arq);

/**
 * @brief Retorna o tamanho de um registro de produto.
 * @return O tamanho do registro de produto.
 */
int tamanho_registro_produto();

/**
 * @brief Gera uma base de dados desordenada de produtos.
 * @param file Ponteiro para o arquivo onde a base será gerada.
 * @param numberRecords Número de registros a serem gerados.
 */
void gerarBaseDesordenada_produto(FILE *file, int numberRecords);

/**
 * @brief Busca um produto por código usando busca sequencial.
 * @param cod Código do produto a ser buscado.
 * @param arq Ponteiro para o arquivo de dados.
 * @return A struct TProd encontrada.
 */
TProd busca_sequencial_produto(int cod, FILE *arq);

/**
 * @brief Busca um produto por código usando busca binária.
 * @param cod Código do produto a ser buscado.
 * @param arq Ponteiro para o arquivo de dados (deve estar ordenado).
 * @param tam Número de registros no arquivo.
 * @return A struct TProd encontrada.
 */
TProd busca_binaria_produto(int cod, FILE *arq, int tam);

/**
 * @brief Ordena um arquivo de produtos usando o método Insertion Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void insertion_sort_disco_produto(FILE *arq, int tam);

/**
 * @brief Ordena um arquivo de produtos usando o método Selection Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void selection_sort_disco_produto(FILE *arq, int tam);

/**
 * @brief Embaralha os registros em um arquivo de produtos.
 * @param arq Ponteiro para o arquivo a ser embaralhado.
 * @param total_records Número total de registros no arquivo.
 */
void embaralhar_arquivo_produto(FILE *arq, int total_records);

#endif // PRODUTO_H_INCLUDED
