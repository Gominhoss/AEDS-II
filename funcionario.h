#ifndef FUNCIONARIOS_FUNCIONARIO_H
#define FUNCIONARIOS_FUNCIONARIO_H

#define FUNCIONARIOS_FILE "funcionarios.dat"

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

typedef struct Funcionario {
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
} TFunc;

typedef struct vetor_func {
  TFunc *func;
  FILE *f;
  TParticao *particao;
  int aux_p;
  int read;

} TVetFunc;

void imprime_funcionario(TFunc *func);

TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);

void salva_funcionario(TFunc *func, FILE *out);

TFunc *le_funcionario(FILE *in);

void le_funcionarios(FILE *in);

int tamanho_funcionario();

int tamanho_arquivo_funcionario(FILE *arq);

int tamanho_registro_funcionario();

TFunc* busca_binaria_funcionario(int cod, FILE *arq, int tam);

TFunc* busca_sequencial_funcionario(int cod, FILE *arq);

void insertion_sort_disco_funcionario(FILE *arq, int tam);

void gerarBaseDesordenada_funcionario(FILE* file, int numberRecords);

void selection_sort_disco_funcionario(FILE *arq, int tam);

void embaralhar_arquivo_funcionario(FILE *arq, int total_records);

void intercala_particoes_funcionarios(TVetFunc *particao,TVetFunc *particao2, TNomes *nome_particoes, int index);

void intercalacao_funcionarios(char *nome_arquivo_saida, TNomes *nome_particoes);

void processa_particoes_funcionarios(TVetFunc **v, TNomes *nome_particoes, int num_p, char *out);

void mergeSort_funcionarios(TVetFunc **arr, int l, int r);

void merge_funcionarios(TVetFunc **arr, int l, int m, int r);

void imprime_cod_func(FILE *in);

void classificacaoSubs_func(FILE *arq);


#endif //FUNCIONARIOS_FUNCIONARIO_H
