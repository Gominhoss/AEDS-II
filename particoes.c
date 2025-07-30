#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "particoes.h"

int has_dat_extension(char const *name) {
  size_t len = strlen(name);
  return len > 4 && strcmp(name + len - 4, ".dat") == 0;
}

int name_starts_with_p(char const *name) {
  size_t len = strlen(name);
  return len > 1 && strncmp(name, "p", 1) == 0;
}

long numero_particao(char *nome_arquivo) {
  char *p = nome_arquivo;
  long val = INT_MAX;
  char *output;

  int i;
  for (i = 0; p[i] != '\0'; i++) {
    if (isdigit(p[i])) {
      // val = strtol(p, &output, 10) + 1;
      val = (int)p[i];
      // printf("%ld\n", val);
    }
  }

  return val;
}

TParticao *le_particao(char *nome) {
  TParticao *particao = (TParticao *)malloc(sizeof(TParticao));
  if (particao) {
    memset(particao, 0, sizeof(TParticao));
  }

  particao->num = numero_particao(nome);
  particao->nome = nome;
  // strcpy(particao->nome, nome);

  return particao;
}

TParticao *criar_particao(int num, char *nome) {
  TParticao *particao = (TParticao *)malloc(sizeof(TParticao));

  if (particao) {
    memset(particao, 0, sizeof(TParticao));
  }

  particao->num = num;
  strcpy(particao->nome, nome);

  return particao;
}

void inicializa_lista(TNomes *lista) {
  lista->primeiro = (TParticao *)malloc(sizeof(TParticao));
  lista->ultimo = lista->primeiro;
  lista->primeiro->prox = NULL;
  lista->tam = 0;
}

int lista_vazia(TNomes lista) { return (lista.primeiro == lista.ultimo); }

void inserir(TParticao p, TNomes *Lista) {
  Lista->ultimo->prox = (TParticao *)malloc(sizeof(TParticao));
  Lista->ultimo = Lista->ultimo->prox;
  Lista->ultimo->nome = strdup(p.nome);
  Lista->ultimo->num = p.num;
  // memcpy(Lista->ultimo->num, p.num, sizeof(int));
  Lista->ultimo->prox = NULL;
  Lista->tam++;

  // printf("Particao %s inserida.\n", Lista -> ultimo -> nome);
}

void insere_particao(TNomes *nome_particoes, char *d_name) {
  TParticao *aux;
  aux = le_particao(d_name);

  if (aux != NULL) {
    inserir(*aux, nome_particoes);
  } else {
    printf("ERROR: Nao foi possivel inserir particao na lista.\n");
  }
}

TNomes *le_nomes_particoes_diretorio(char *nomePasta) {
    TNomes *nomes_particoes = (TNomes *)malloc(sizeof(TNomes));
    inicializa_lista(nomes_particoes);
    DIR *dr;
    struct dirent *en;

    // char *nome_dir = malloc(50 * sizeof(char));
    char nome_dir[50];
    strcpy(nome_dir, "./");
    strcat(nome_dir, nomePasta);

    // printf("%s", nome_dir);
    dr = opendir(nome_dir);

  if (dr) {
    while ((en = readdir(dr)) != NULL) {
      if ((has_dat_extension(en->d_name))) {
        // printf("\nEncontrou arquivo .dat\n");
        // printf("%s\n", en -> d_name);
        if ((name_starts_with_p(en->d_name))) {
          insere_particao(nomes_particoes, en->d_name);
          // printf("\nParticao encontrada: %s\n", en -> d_name);
        }
      }
    }
    closedir(dr);
  }

  printf("Leitura do diretorio finalizada.\n");
  return nomes_particoes;
}

void mostrar_particoes(TNomes lista) {
  TParticao *aux = lista.primeiro->prox;
  printf("\n--------- LISTA DE PARTICOES -------\n");
  printf("TOTAL: %d\n", lista.tam);

  while (aux != NULL) {
    print_particao(*aux);
    aux = aux->prox;
  }
  free(aux);
}

void remover_particoes(TNomes *lista) {
  int tamanho = lista->tam;
  TParticao *aux = lista->primeiro->prox;

  while (aux != NULL) {

    remove(aux->nome);
    // printf("\nParticao removida: %s\n", aux -> nome);
    aux = aux->prox;
  }

  free(aux);
}

TParticao* pesquisar (TNomes Lista, TParticao particao)
{
    TParticao* aux;
    aux = Lista.primeiro;

    while(aux -> prox != NULL)
    {
        if (aux -> prox -> num == particao.num)
        {
            return aux;
        }
        aux = aux -> prox;
    }
    return NULL;
}

void excluir_particao (TNomes *Lista, TParticao *particao)
{
    TParticao *aux1, *aux2;
    aux1 = pesquisar(*Lista, *particao);

    if (aux1 != NULL)
    {
        aux2 = aux1 -> prox;
        aux1 -> prox = aux2 -> prox;
        *particao = *aux2;
        if (aux1 -> prox == NULL)
        {
            Lista -> ultimo = aux1;
        }
        free (aux2);
        Lista -> tam--;
    }
}

int verifica_particoes_existentes(char* nomePasta){
  TNomes* particoes_existentes = (TNomes *)malloc(sizeof(TNomes));
  particoes_existentes = le_nomes_particoes_diretorio(nomePasta);
  // mostrar_particoes(*particoes_existentes);
  printf("Particoes no diretorio: %d\n", particoes_existentes -> tam);

  return (particoes_existentes -> tam > 0);
}

void apaga_particoes_existentes(char* nomePasta){
  if (verifica_particoes_existentes(nomePasta) == 1){
    TNomes* particoes_existentes = (TNomes *)malloc(sizeof(TNomes));
    particoes_existentes = le_nomes_particoes_diretorio(nomePasta);
    remover_particoes(particoes_existentes);

    free(particoes_existentes);

  } else {
    printf("Nao existem particoes pendentes no diretorio.\n\n");
  }
}

void print_particao(TParticao p) {
  printf("NOME: %s\n", p.nome);
  printf("NUMERO: %ld\n", p.num);

  if (p.prox == NULL) {
    printf("PROX: NULL\n\n");
  } else {
    printf("PROX: %s\n\n", p.prox->nome);
  }
}
