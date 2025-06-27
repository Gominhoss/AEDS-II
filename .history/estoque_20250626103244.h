#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


#define ESTOQUE_FILE "estoque.dat"

typedef struct Estoque {
  int cod;
  int atual;
  int capacidade;
  int min;
  // TLista produtos;

} TEstoque;

void imprime_estoque(TEstoque *estoque);

TEstoque *estoque(int cod, int atual, int capacidade, int min);

void salva_estoque(TEstoque *estoque, FILE *out);

TEstoque *le_estoque(FILE *in);

void le_estoques(FILE *in);

int tamanho_estoque();

int tamanho_arquivo_estoque(FILE *arq);

int tamanho_registro_estoque();

void gerarBaseDesordenada_estoque(FILE *file, int numberRecords);

TEstoque busca_sequencial_estoque(int cod, FILE *arq);

TEstoque busca_binaria_estoque(int cod, FILE *arq, int tam);

void insertion_sort_disco_estoque(FILE *arq, int tam);

void selection_sort_disco_estoque(FILE *arq, int tam);

#endif