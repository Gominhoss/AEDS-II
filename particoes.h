#ifndef PARTICOES_H_INCLUDED
#define PARTICOES_H_INCLUDED

#include <dirent.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <ctype.h>

typedef struct particao {
  long num;
  char *nome;
  struct particao *prox;

} TParticao;

typedef struct nomes {
  int tam;
  struct particao *primeiro;
  struct particao *ultimo;

} TNomes;


int has_dat_extension(char const *name);
int name_starts_with_p(char const *name);
long numero_particao(char *nome_arquivo);
TParticao *le_particao(char *nome);
TParticao *criar_particao(int num, char *nome);
void inicializa_lista(TNomes *lista);
int lista_vazia(TNomes lista);
void inserir(TParticao p, TNomes *Lista);
void insere_particao(TNomes *nome_particoes, char *d_name);
TNomes *le_nomes_particoes_diretorio(char *nomePasta);
void print_particao(TParticao p);
void mostrar_particoes(TNomes lista);
void remover_particoes(TNomes *lista);
TParticao* pesquisar (TNomes Lista, TParticao particao);
void excluir_particao (TNomes *Lista, TParticao *particao);
int verifica_particoes_existentes(char* nomePasta);
void apaga_particoes_existentes(char* nomePasta);
void print_particao(TParticao p);

#endif // PARTICOES_H_INCLUDED