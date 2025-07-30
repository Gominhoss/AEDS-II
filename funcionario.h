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

typedef struct {
    int index;
    TFunc *funcionario;
} TVencedorFunc;

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

int classificacaoSubs_func(FILE *arq);

int compara_funcionarios(const TFunc *f1, const TFunc *f2);

void intercalacao_funcionarios_vencedores(char *nome_arquivo_saida, int num_p);

#endif //FUNCIONARIOS_FUNCIONARIO_H
