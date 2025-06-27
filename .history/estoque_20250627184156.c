#include "estoque.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

void imprime_estoque(TEstoque *estoque) {
  printf("\n*************** ESTOQUE *****************");
  printf("\nCODIGO: ");
  printf("%d", estoque->cod);
  printf("\nATUAL: ");
  printf("%d", estoque->atual);
  printf("\nCAPACIDADE: ");
  printf("%d", estoque->capacidade);
  printf("\nMINIMO: ");
  printf("%d", estoque->min);
}

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

void salva_estoque(TEstoque *estoque, FILE *out) {
  fwrite(&estoque->cod, sizeof(int), 1, out);
  fwrite(&estoque->atual, sizeof(int), 1, out);
  fwrite(&estoque->capacidade, sizeof(int), 1, out);
  fwrite(&estoque->min, sizeof(int), 1, out);
}

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

void le_estoques(FILE *in) {
  printf("\n\nLendo estoques do arquivo...\n\n");
  rewind(in);
  TEstoque estoque;

  while (fread(&estoque, sizeof(TEstoque), 1, in) == 1) {
    imprime_estoque(&estoque);
  }
}

int tamanho_estoque() {
  return sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int);
}

int tamanho_arquivo_estoque(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_estoque());
  return tam;
}

int tamanho_registro_estoque() { return sizeof(TEstoque); }

void embaralhar_arquivo_estoque(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        // Troca os registros nas posições i e j
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

void gerarBaseDesordenada_estoque(FILE *file, int numberRecords) {
  
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < numberRecords; i++) {
        TEstoque estoque;
        estoque.cod = i + 1;
        estoque.atual = 50 + (i * 2);
        estoque.capacidade = 500;
        estoque.min = 50;
        salva_estoque(&estoque, file);
    }
    fflush(file);
    
    embaralhar_arquivo_estoque(file, numberRecords);
}

TEstoque busca_sequencial_estoque(int cod, FILE *arq) {
  double tempoTotal = 0;
  int comp = 0;
  int i = 0;
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  rewind(arq);
  TEstoque estoque;

  while (fread(&estoque, sizeof(TEstoque), 1, arq) == 1) {
    if (cod == estoque.cod) {
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return estoque;
      break;
    } else {
      comp++;
    }
  }
  printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  printf("\nComparacoes = %d\n", comp);
  return estoque;
}

TEstoque busca_binaria_estoque(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  double tempoTotal = 0;
  TEstoque estoque;

  rewind(arq);
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_estoque(), SEEK_SET);
    fread(&estoque, sizeof(TEstoque), 1, arq);
    if (cod == estoque.cod) {
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return estoque;
    } else if (estoque.cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  printf("\nComparacoes = %d\n", comp);
  return estoque;
}

void insertion_sort_disco_estoque(FILE *arq, int tam) {
  int i;
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
      fseek(arq, (i - 1) * tamanho_registro_estoque(), SEEK_SET);
      fi = le_estoque(arq);
    }
    fseek(arq, (i)*tamanho_registro_estoque(), SEEK_SET);
    salva_estoque(fj, arq);
  }
  fflush(arq);
}

void selection_sort_disco_estoque(FILE *arq, int tam) {
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
    fflush(arq);
}
