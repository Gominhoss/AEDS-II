#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#define CLIENTES_FILE "clientes.dat"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

typedef struct Cliente {
  int cod;
  char nome[50];
  char cpf[15];
  char data_nascimento[11];
  char contato[20];
} TCliente;

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

#endif // CLIENTE_H_INCLUDED