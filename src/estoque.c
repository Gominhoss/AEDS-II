/**
 * @file estoque.c
 * @brief Implementação das funções para manipulação de dados de estoque.
 */

#include "estoque.h"
#include "produto.h"
#include "log.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_REGISTROS 10;

// Imprime os dados de um item de estoque
void imprime_estoque(TEstoque *estoque) {
  printf("\n*************** ESTOQUE *****************");
  printf("\nCODIGO: ");
  printf("%d", estoque->cod);
  printf("\nCAPACIDADE: ");
  printf("\n   - ATUAL: ");
  printf("%d", estoque->atual);
  printf("\n   - TOTAL: ");
  printf("%d", estoque->capacidade);
  printf("\n   - MINIMA: ");
  printf("%d", estoque->min);
}

// Cria uma nova estrutura TEstoque em memória
TEstoque *estoque(int cod, int atual, int capacidade, int min) {
  TEstoque *estoque = (TEstoque *)malloc(sizeof(TEstoque));

  if (estoque)
    memset(estoque, 0, sizeof(TEstoque));

  estoque->cod = cod;
  estoque->atual = atual;
  estoque->capacidade = capacidade;
  estoque->min = min;

  return estoque;
}

// Salva os dados de um item de estoque em um arquivo
void salva_estoque(TEstoque *estoque, FILE *out) {
  fwrite(&estoque->cod, sizeof(int), 1, out);
  fwrite(&estoque->atual, sizeof(int), 1, out);
  fwrite(&estoque->capacidade, sizeof(int), 1, out);
  fwrite(&estoque->min, sizeof(int), 1, out);
}

// Lê os dados de um item de estoque de um arquivo
TEstoque *le_estoque(FILE *in) {
  TEstoque *estoque = (TEstoque *)malloc(sizeof(TEstoque));

  if (0 >= fread(&estoque->cod, sizeof(int), 1, in)) {
    free(estoque);
    return NULL;
  }
  fread(&estoque->atual, sizeof(int), 1, in);
  fread(&estoque->capacidade, sizeof(int), 1, in);
  fread(&estoque->min, sizeof(int), 1, in);

  return estoque;
}

// Lê e imprime todos os itens de estoque de um arquivo
void le_estoques(FILE *in) {
  printf("\n\nLendo estoques do arquivo...\n\n");
  rewind(in);
  TEstoque estoque;

  while (fread(&estoque, sizeof(TEstoque), 1, in) == 1) {
    imprime_estoque(&estoque);
  }
}

// Retorna o tamanho da struct TEstoque em bytes
int tamanho_estoque() {
  return sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int);
}

// Calcula o número de registros de estoque em um arquivo
int tamanho_arquivo_estoque(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_estoque());
  return tam;
}

// Retorna o tamanho de um registro de estoque
int tamanho_registro_estoque() { return sizeof(TEstoque); }

// Embaralha os registros em um arquivo de estoque
void embaralhar_arquivo_estoque(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        fseek(arq, i * tamanho_registro_estoque(), SEEK_SET);
        TEstoque *reg_i = le_estoque(arq);

        fseek(arq, j * tamanho_registro_estoque(), SEEK_SET);
        TEstoque *reg_j = le_estoque(arq);

        fseek(arq, i * tamanho_registro_estoque(), SEEK_SET);
        salva_estoque(reg_j, arq);

        fseek(arq, j * tamanho_registro_estoque(), SEEK_SET);
        salva_estoque(reg_i, arq);

        free(reg_i);
        free(reg_j);
    }
    fflush(arq);
}

// Lista todos os estoques que estão com a quantidade atual abaixo da mínima
void listar_estoques_abaixo_do_minimo(FILE *arq) {
  rewind(arq);
  TEstoque *est;

  printf("\n-------- Estoques abaixo do minimo --------\n");

  while ((est = le_estoque(arq)) != NULL) {
    if (est->atual < est->min) {
      imprime_estoque(est);
    }
    free(est);
  }
}

// Gera uma base de dados desordenada de estoque a partir de um arquivo de produtos
void gerarBaseDesordenada_estoque(FILE *fileEstoque, FILE *fileProdutos) {
    srand(time(NULL));

    rewind(fileProdutos);
    fseek(fileEstoque, 0, SEEK_SET);

    TProd *prod;
    while ((prod = le_produto(fileProdutos)) != NULL) {
        TEstoque estoque;
        estoque.cod = prod->cod;
        estoque.atual = rand() % 501;
        estoque.capacidade = 500;
        estoque.min = 50;

        salva_estoque(&estoque, fileEstoque);

        free(prod);
    }

    fflush(fileEstoque);

    fseek(fileEstoque, 0, SEEK_END);
    int numRegistros = ftell(fileEstoque) / tamanho_registro_estoque();

    embaralhar_arquivo_estoque(fileEstoque, numRegistros);
}

// Busca um item de estoque por código usando busca sequencial
TEstoque busca_sequencial_estoque(int cod, FILE *arq) {
  int comp = 0;
  TEstoque estoque;
  iniciar_medicao();

  rewind(arq);

  while (fread(&estoque, sizeof(TEstoque), 1, arq) == 1) {
    comp++;
    if (cod == estoque.cod) {
      double tempo = finalizar_medicao();
      registrar_tempo_execucao("Busca Sequencial (Estoque)", tempo);
      printf("\nComparacoes = %d\n", comp);
      return estoque;
    }
  }
  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Busca Sequencial (Estoque) - Nao encontrado", tempo);
  printf("\nComparacoes = %d\n", comp);
  estoque.cod = -1; // Indica que não foi encontrado
  return estoque;
}

// Busca um item de estoque por código usando busca binária
TEstoque busca_binaria_estoque(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  TEstoque estoque;
  iniciar_medicao();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_estoque(), SEEK_SET);
    fread(&estoque, sizeof(TEstoque), 1, arq);
    comp++;
    if (cod == estoque.cod) {
      double tempo = finalizar_medicao();
      registrar_tempo_execucao("Busca Binaria (Estoque)", tempo);
      printf("\nComparacoes = %d\n", comp);
      return estoque;
    } else if (estoque.cod < cod) {
      left = middle + 1;
    } else {
      right = middle - 1;
    }
  }

  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Busca Binaria (Estoque) - Nao encontrado", tempo);
  printf("\nComparacoes = %d\n", comp);
  estoque.cod = -1; // Indica que não foi encontrado
  return estoque;
}

// Ordena um arquivo de estoque usando o método Insertion Sort em disco
void insertion_sort_disco_estoque(FILE *arq, int tam) {
  int i;
  iniciar_medicao();
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_estoque(), SEEK_SET);
    TEstoque *fj = le_estoque(arq);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_estoque(), SEEK_SET);
    TEstoque *fi = le_estoque(arq);
    while ((i > 0) && (fi->cod > fj->cod)) {
      fseek(arq, i * tamanho_registro_estoque(), SEEK_SET);
      salva_estoque(fi, arq);
      i = i - 1;
      if (i > 0) {
          fseek(arq, (i - 1) * tamanho_registro_estoque(), SEEK_SET);
          free(fi);
          fi = le_estoque(arq);
      }
    }
    fseek(arq, i * tamanho_registro_estoque(), SEEK_SET);
    salva_estoque(fj, arq);
    free(fi);
    free(fj);
  }
  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Insertion Sort Disco (Estoque)", tempo);
  fflush(arq);
}

// Ordena um arquivo de estoque usando o método Selection Sort em disco
void selection_sort_disco_estoque(FILE *arq, int tam) {
  iniciar_medicao();
  for (int i = 0; i < tam - 1; i++) {
        int min_idx = i;
        fseek(arq, i * tamanho_registro_estoque(), SEEK_SET);
        TEstoque *min_estoque = le_estoque(arq);

        for (int j = i + 1; j < tam; j++) {
            fseek(arq, j * tamanho_registro_estoque(), SEEK_SET);
            TEstoque *estoque_j = le_estoque(arq);

            if (estoque_j->cod < min_estoque->cod) {
                free(min_estoque);
                min_estoque = estoque_j;
                min_idx = j;
            } else {
                free(estoque_j);
            }
        }

        if (min_idx != i) {
            fseek(arq, i * tamanho_registro_estoque(), SEEK_SET);
            TEstoque *estoque_i = le_estoque(arq);

            fseek(arq, i * tamanho_registro_estoque(), SEEK_SET);
            salva_estoque(min_estoque, arq);

            fseek(arq, min_idx * tamanho_registro_estoque(), SEEK_SET);
            salva_estoque(estoque_i, arq);

            free(estoque_i);
        }
        free(min_estoque);
    }

    double tempoTotal = finalizar_medicao();
    registrar_tempo_execucao("SelectionSort (Estoque)", tempoTotal);
    fflush(arq);
}

