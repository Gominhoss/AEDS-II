/**
 * @file cliente.h
 * @brief Definições de tipos e protótipos de funções para manipulação de dados de clientes.
 *
 * Este arquivo contém a definição da struct TCliente, que representa um cliente,
 * e os protótipos das funções para criar, salvar, ler, ordenar e buscar clientes em arquivos.
 */

#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#define CLIENTES_FILE "clientes.dat"

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/**
 * @brief Estrutura para representar um cliente.
 */
typedef struct Cliente {
  int cod;                  /**< Código único do cliente. */
  char nome[50];            /**< Nome do cliente. */
  char cpf[15];             /**< CPF do cliente. */
  char data_nascimento[11]; /**< Data de nascimento do cliente (formato DD/MM/AAAA). */
  char contato[20];         /**< Contato do cliente (email ou telefone). */
} TCliente;

/**
 * @brief Imprime os dados de um cliente na saída padrão.
 * @param cliente Ponteiro para a struct TCliente a ser impressa.
 */
void imprime_cliente(TCliente *cliente);

/**
 * @brief Cria uma nova estrutura TCliente em memória.
 * @param cod Código do cliente.
 * @param nome Nome do cliente.
 * @param cpf CPF do cliente.
 * @param data_nascimento Data de nascimento do cliente.
 * @param contato Contato do cliente.
 * @return Ponteiro para a struct TCliente criada.
 */
TCliente *cliente(int cod, char *nome, char *cpf, char *data_nascimento, char *contato);

/**
 * @brief Salva os dados de um cliente em um arquivo.
 * @param cliente Ponteiro para a struct TCliente a ser salva.
 * @param out Ponteiro para o arquivo onde os dados serão escritos.
 */
void salva_cliente(TCliente *cliente, FILE *out);

/**
 * @brief Lê os dados de um cliente de um arquivo.
 * @param in Ponteiro para o arquivo de onde os dados serão lidos.
 * @return Ponteiro para a struct TCliente lida do arquivo, ou NULL se o fim do arquivo for alcançado.
 */
TCliente *le_cliente(FILE *in);

/**
 * @brief Lê e imprime todos os clientes de um arquivo.
 * @param in Ponteiro para o arquivo a ser lido.
 */
void le_clientes(FILE *in);

/**
 * @brief Calcula o tamanho em bytes da struct TCliente.
 * @return O tamanho da struct TCliente.
 */
int tamanho_cliente();

/**
 * @brief Calcula o número de registros de clientes em um arquivo.
 * @param arq Ponteiro para o arquivo.
 * @return O número de registros no arquivo.
 */
int tamanho_arquivo_cliente(FILE *arq);

/**
 * @brief Retorna o tamanho de um registro de cliente (equivalente a sizeof(TCliente)).
 * @return O tamanho do registro de cliente.
 */
int tamanho_registro_cliente();

/**
 * @brief Gera uma base de dados desordenada de clientes.
 * @param file Ponteiro para o arquivo onde a base será gerada.
 * @param numberRecords Número de registros a serem gerados.
 */
void gerarBaseDesordenada_cliente(FILE *file, int numberRecords);

/**
 * @brief Busca um cliente por código usando busca sequencial.
 * @param cod Código do cliente a ser buscado.
 * @param arq Ponteiro para o arquivo de dados.
 * @return Ponteiro para o cliente encontrado, ou NULL se não for encontrado.
 */
TCliente *busca_sequencial_cliente(int cod, FILE *arq);

/**
 * @brief Busca um cliente por código usando busca binária.
 * @param cod Código do cliente a ser buscado.
 * @param arq Ponteiro para o arquivo de dados (deve estar ordenado).
 * @param tam Número de registros no arquivo.
 * @return Ponteiro para o cliente encontrado, ou NULL se não for encontrado.
 */
TCliente *busca_binaria_cliente(int cod, FILE *arq, int tam);

/**
 * @brief Ordena um arquivo de clientes usando o método Insertion Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void insertion_sort_disco_cliente(FILE *arq, int tam);

/**
 * @brief Ordena um arquivo de clientes usando o método Selection Sort em disco.
 * @param arq Ponteiro para o arquivo a ser ordenado.
 * @param tam Número de registros no arquivo.
 */
void selection_sort_disco_cliente(FILE *arq, int tam);

/**
 * @brief Embaralha os registros em um arquivo de clientes.
 * @param arq Ponteiro para o arquivo a ser embaralhado.
 * @param total_records Número total de registros no arquivo.
 */
void embaralhar_arquivo_cliente(FILE *arq, int total_records);

#endif // CLIENTE_H_INCLUDED