#ifndef FORNECEDOR_H_INCLUDED
#define FORNECEDOR_H_INCLUDED

#define FORNECEDOR_FILE "fornecedores.dat"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

typedef struct Fornecedor {
  int cod;
  char nome[50];
  char cnpj[19];
} TForn;

typedef struct vetor_forn {
  TForn *en;
  FILE *f;
  TParticao *particao;
  int aux_p;
  int read;

} TVetForn;


void imprime_fornecedor(TForn *forn);

TForn *fornecedor(int cod, char *nome, char *cnpj);

void salva_fornecedor(TForn *forn, FILE *out);

TForn *le_fornecedor(FILE *in);

void le_fornecedores(FILE *in);

int tamanho_fornecedor();

int tamanho_arquivo_fornecedor(FILE *arq);

int tamanho_registro_fornecedor();

void gerarBaseDesordenada_fornecedor(FILE *file, int numberRecords);

TForn busca_sequencial_fornecedor(int cod, FILE *arq);

TForn busca_binaria_fornecedor(int cod, FILE *arq, int tam);

void insertion_sort_disco_fornecedor(FILE *arq, int tam);

void selection_sort_disco_fornecedor(FILE *arq, int tam);

void embaralhar_arquivo_fornecedor(FILE *arq, int total_records);

void classificacaoSubs_forn(FILE *arq);

void imprime_cod_forn(FILE *in);

void merge_fornecedores(TVetForn **arr, int l, int m, int r); 

void intercalacao_fornecedores(char *nome_arquivo_saida, TNomes *nome_particoes); 

void processa_particoes_fornecedores(TVetForn **v, TNomes *nome_particoes, int num_p, char *out); 

void intercala_particoes_fornecedores(TVetForn *particao,TVetForn *particao2, TNomes *nome_particoes, int index);

void mergeSort_fornecedores(TVetForn **arr, int l, int r);

#endif // FORNECEDOR_H_INCLUDED