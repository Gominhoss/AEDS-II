#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#define CLIENTES_FILE "clientes.dat"

#include <stdio.h>
#include <stdlib.h>
#include "particoes.h"
#include "utils.h"

typedef struct Cliente {
  int cod;
  char nome[50];
  char cpf[15];
  char data_nascimento[11];
  char contato[20];
} TCliente;

typedef struct vetor {
  TCliente *cli;
  FILE *f;
  TParticao *particao;
  int aux_p;
  int read;

} TVetClientes;


void imprime_cliente(TCliente *cliente);

TCliente *cliente(int cod, char *nome, char *cpf, char *data_nascimento,
                  char *contato);

void salva_cliente(TCliente *cliente, FILE *out);

TCliente *le_cliente(FILE *in);

void le_clientes(FILE *in);

int tamanho_cliente();

int tamanho_arquivo_cliente(FILE *arq);

int tamanho_registro_cliente();

void gerarBaseDesordenada_cliente(FILE *file, int numberRecords);

TCliente *busca_sequencial_cliente(int cod, FILE *arq);

TCliente *busca_binaria_cliente(int cod, FILE *arq, int tam);

void insertion_sort_disco_cliente(FILE *arq, int tam);

void selection_sort_disco_cliente(FILE *arq, int tam);

void embaralhar_arquivo_cliente(FILE *arq, int total_records);

void classificacaoSubs_cli(FILE *arq);

void imprime_cod_cli(FILE *in);

void merge_clientes(TVetClientes **arr, int l, int m, int r);

void intercalacao_clientes(char *nome_arquivo_saida, TNomes *nome_particoes);

void processa_particoes_clientes(TVetClientes **v, TNomes *nome_particoes,int num_p, char *out);

void intercala_particoes_clientes(TVetClientes *particao, TVetClientes *particao2, TNomes *nome_particoes, int index);

#endif // CLIENTE_H_INCLUDED