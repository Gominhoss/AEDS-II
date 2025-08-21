/**
 * @file estoque.h
 * @brief Definições de tipos e protótipos de funções para manipulação de dados de estoque.
 *
 * Este arquivo contém a definição da struct TEstoque e os protótipos das funções
 * para gerenciar o estoque de produtos, incluindo criação, salvamento, leitura, ordenação e busca.
 */

#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "produto.h"
#include "log.h"

#define ESTOQUE_FILE "estoque.dat"

/**
 * @brief Estrutura para representar o estoque de um produto.
 */
typedef struct Estoque {
  int cod;        /**< Código do produto associado ao estoque. */
  int atual;      /**< Quantidade atual do produto em estoque. */
  int capacidade; /**< Capacidade máxima de armazenamento do produto. */
  int min;        /**< Quantidade mínima recomendada em estoque. */
} TEstoque;

/**
 * @brief Imprime os dados de um item de estoque.
 * @param estoque Ponteiro para a struct TEstoque a ser impressa.
 */
void imprime_estoque(TEstoque *estoque);

/**
 * @brief Cria uma nova estrutura TEstoque em memória.
 * @param cod Código do produto.
 * @param atual Quantidade atual.
 * @param capacidade Capacidade máxima.
 * @param min Quantidade mínima.
 * @return Ponteiro para a struct TEstoque criada.
 */
TEstoque *estoque(int cod, int atual, int capacidade, int min);

/**
 * @brief Salva os dados de um item de estoque em um arquivo.
 * @param estoque Ponteiro para a struct TEstoque a ser salva.
 * @param out Ponteiro para o arquivo de saída.
 */
void salva_estoque(TEstoque *estoque, FILE *out);

/**
 * @brief Lê os dados de um item de estoque de um arquivo.
 * @param in Ponteiro para o arquivo de entrada.
 * @return Ponteiro para a struct TEstoque lida, ou NULL se o fim do arquivo for alcançado.
 */
TEstoque *le_estoque(FILE *in);

/**
 * @brief Lê e imprime todos os itens de estoque de um arquivo.
 * @param in Ponteiro para o arquivo a ser lido.
 */
void le_estoques(FILE *in);

/**
 * @brief Calcula o tamanho em bytes da struct TEstoque.
 * @return O tamanho da struct TEstoque.
 */
int tamanho_estoque();

/**
 * @brief Calcula o número de registros de estoque em um arquivo.
 * @param arq Ponteiro para o arquivo.
 * @return O número de registros no arquivo.
 */
int tamanho_arquivo_estoque(FILE *arq);

/**
 * @brief Retorna o tamanho de um registro de estoque.
 * @return O tamanho do registro de estoque.
 */
int tamanho_registro_estoque();

/**
 * @brief Lista todos os estoques que estão com a quantidade atual abaixo da mínima recomendada.
 * @param arq Ponteiro para o arquivo de estoque.
 */
void listar_estoques_abaixo_do_minimo(FILE *arq);

/**
 * @brief Gera uma base de dados desordenada de estoque, baseada em um arquivo de produtos.
 * @param fileEstoque Arquivo onde a base de estoque será gerada.
 * @param fileProdutos Arquivo de produtos que servirá de base para os códigos.
 */
void gerarBaseDesordenada_estoque(FILE *fileEstoque, FILE *fileProdutos);

/**
 * @brief Busca um item de estoque por código usando busca sequencial.
 * @param cod Código do item a ser buscado.
 * @param arq Ponteiro para o arquivo de dados.
 * @return A struct TEstoque encontrada.
 */
TEstoque busca_sequencial_estoque(int cod, FILE *arq);

/**
 * @brief Busca um item de estoque por código usando busca binária.
 * @param cod Código do item a ser buscado.
 * @param arq Ponteiro para o arquivo de dados (deve estar ordenado).
 * @param tam Número de registros no arquivo.
 * @return A struct TEstoque encontrada.
 */
TEstoque busca_binaria_estoque(int cod, FILE *arq, int tam);

/**
 * @brief Ordena um arquivo de estoque usando o método Insertion Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void insertion_sort_disco_estoque(FILE *arq, int tam);

/**
 * @brief Ordena um arquivo de estoque usando o método Selection Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void selection_sort_disco_estoque(FILE *arq, int tam);

/**
 * @brief Embaralha os registros em um arquivo de estoque.
 * @param arq Ponteiro para o arquivo a ser embaralhado.
 * @param total_records Número total de registros no arquivo.
 */
void embaralhar_arquivo_estoque(FILE *arq, int total_records);

#endif