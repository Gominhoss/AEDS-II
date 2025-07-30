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

void listar_estoques_abaixo_do_minimo(FILE *arq);

void gerarBaseDesordenada_estoque(FILE *fileEstoque, FILE *fileProdutos);

TEstoque busca_sequencial_estoque(int cod, FILE *arq);

TEstoque busca_binaria_estoque(int cod, FILE *arq, int tam);

void insertion_sort_disco_estoque(FILE *arq, int tam);

void selection_sort_disco_estoque(FILE *arq, int tam);

void embaralhar_arquivo_estoque(FILE *arq, int total_records);

int classificacaoSubs_estq(FILE *arq);

int compara_estoque(TEstoque *e1, TEstoque *e2);

void intercalacao_estoque_vencedores(char *nome_arquivo_saida, int num_p);

#endif