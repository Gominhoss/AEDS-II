#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED

#define PRODUTOS_FILE "produtos.dat"

#include <stdio.h>
#include <stdlib.h>

#include "entidades.h"

typedef struct Produto {
  int cod;
  char nome[50];
  char valor[50];
  int cod_forn;
} TProd;

typedef struct vetor_prod {
  TProd *en;
  FILE *f;
  TParticao *particao;
  int aux_p;
  int read;

} TVetProd;

typedef struct {
    int index;
    TProd *produto;
} TVencedorProduto;

void imprime_produto(TProd *prod);

TProd *produto(int cod, char *nome, char *valor, int cod_forn);

void salva_produto(TProd *prod, FILE *out);

TProd *le_produto(FILE *in);

void le_produtos(FILE *in);

int tamanho_produto();

int tamanho_arquivo_produto(FILE *arq);

int tamanho_registro_produto();

void gerarBaseDesordenada_produto(FILE *file, int numberRecords);

TProd busca_sequencial_produto(int cod, FILE *arq);

TProd busca_binaria_produto(int cod, FILE *arq, int tam);

void insertion_sort_disco_produto(FILE *arq, int tam);

void selection_sort_disco_produto(FILE *arq, int tam);

void embaralhar_arquivo_produto(FILE *arq, int total_records);

void imprime_cod_prod(FILE *in);

int classificacaoSubs_prod(FILE *arq);

int compara_produtos(TProd *p1, TProd *p2);

void intercalacao_produtos_vencedores(char *nome_arquivo_saida, int num_p);

#endif // PRODUTO_H_INCLUDED
