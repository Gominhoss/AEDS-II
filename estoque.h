#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <stdio.h>
#include <stdlib.h>
#include "particoes.h"
#include "utils.h"
#include "produto.h"

#define ESTOQUE_FILE "estoque.dat"

typedef struct Estoque {
  int cod;
  int atual;
  int capacidade;
  int min;
  // TLista produtos;

} TEstoque;

typedef struct vetor_est {
  TEstoque *en;
  FILE *f;
  TParticao *particao;
  int aux_p;
  int read;

} TVetEstoque;


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

void embaralhar_arquivo_estoque(FILE *arq, int total_records);

void classificacaoSubs_estq(FILE *arq);

void mergeSort_estoque(TVetEstoque **arr, int l, int r);

void intercalacao_estoque(char *nome_arquivo_saida, TNomes *nome_particoes);

void processa_particoes_estoque(TVetEstoque **v, TNomes *nome_particoes, int num_p, char *out);

void intercala_particoes_estoque(TVetEstoque *particao, TVetEstoque *particao2, TNomes *nome_particoes, int index);

void classificacaoSubs_estoq(FILE *arq);

#endif