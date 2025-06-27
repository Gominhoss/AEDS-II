#include "fornecedor.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void imprime_fornecedor(TForn *forn) {
  printf("\n**********************************************");
  printf("\nFornecedor de codigo ");
  printf("%d", forn->cod);
  printf("\nNome: ");
  printf("%s", forn->nome);
  printf("\nCNPJ: ");
  printf("%s", forn->cnpj);
  printf("\n**********************************************");
}

TForn *fornecedor(int cod, char *nome, char *cnpj) {
  TForn *forn = (TForn *)malloc(sizeof(TForn));
  if (forn)
    memset(forn, 0, sizeof(TForn));
  forn->cod = cod;
  strcpy(forn->nome, nome);
  strcpy(forn->cnpj, cnpj);

  return forn;
}

void salva_fornecedor(TForn *forn, FILE *out) {
  fwrite(&forn->cod, sizeof(int), 1, out);
  fwrite(forn->nome, sizeof(char), sizeof(forn->nome), out);
  fwrite(forn->cnpj, sizeof(char), sizeof(forn->cnpj), out);
}

TForn *le_fornecedor(FILE *in) {
  TForn *forn = (TForn *)malloc(sizeof(TForn));
  if (0 >= fread(&forn->cod, sizeof(int), 1, in)) {
    free(forn);
    return NULL;
  }
  fread(forn->nome, sizeof(char), sizeof(forn->nome), in);
  fread(forn->cnpj, sizeof(char), sizeof(forn->cnpj), in);
  return forn;
}
/*
void le_fornecedores(FILE *in) {
    printf("\n\nLendo fornecedores do arquivo...\n\n");
    fseek(in, 0, SEEK_SET);
    TForn *f;
    while ((f = le_fornecedor(in)) != NULL) {
        imprime_fornecedor(f);
        free(f);
    }
}
*/

void le_fornecedores(FILE *in) {
  printf("\n\nLendo fornecedores do arquivo...\n\n");
  rewind(in);
  TForn forn;

  while (fread(&forn, sizeof(TForn), 1, in) == 1) {
    imprime_fornecedor(&forn);
  }
}

int tamanho_fornecedor() {
  return sizeof(int)            // cod
         + sizeof(char) * 50    // nome
         + (sizeof(char) * 19); // cnpj
}

int tamanho_arquivo_fornecedor(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_fornecedor());
  return tam;
}

int tamanho_registro_fornecedor() { return sizeof(TForn); }

void embaralhar_arquivo_fornecedor(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        // Troca os registros nas posições i e j
        fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
        TForn *reg_i = le_fornecedor(arq);

        fseek(arq, j * tamanho_registro_fornecedor(), SEEK_SET);
        TForn *reg_j = le_fornecedor(arq);

        fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
        salva_fornecedor(reg_j, arq);

        fseek(arq, j * tamanho_registro_fornecedor(), SEEK_SET);
        salva_fornecedor(reg_i, arq);

        free(reg_i);
        free(reg_j);
    }
    fflush(arq);
}

void gerarBaseDesordenada_fornecedor(FILE *file, int numberRecords) {
  fseek(file, 0, SEEK_SET);
  for (int i = 0; i < numberRecords; i++) {
      TForn forn;
      forn.cod = i + 1;
      sprintf(forn.nome, "Fornecedor %d", forn.cod);
      sprintf(forn.cnpj, "11.111.111/0001-%02d", forn.cod % 100);
      salva_fornecedor(&forn, file);
  }
  fflush(file);

  embaralhar_arquivo_fornecedor(file, numberRecords);
}

TForn busca_sequencial_fornecedor(int cod, FILE *arq) {
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
  TForn forn;

  while (fread(&forn, sizeof(TForn), 1, arq) == 1) {
    if (cod == forn.cod) {
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return forn;
      break;
    } else {
      comp++;
    }
  }
  printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  printf("\nComparacoes = %d\n", comp);

  return forn;
}

TForn busca_binaria_fornecedor(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  long tempoTotal = 0;
  TForn forn;

  rewind(arq);
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_fornecedor(), SEEK_SET);
    fread(&forn, sizeof(TForn), 1, arq);
    if (cod == forn.cod) {
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %ld segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return forn;
    } else if (forn.cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  printf("\nComparacoes = %d\n", comp);
  return forn;
}

void insertion_sort_disco_fornecedor(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_fornecedor(), SEEK_SET);
    TForn *fj = le_fornecedor(arq);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_fornecedor(), SEEK_SET);
    TForn *fi = le_fornecedor(arq);
    while ((i > 0) && (fi->cod > fj->cod)) {
      fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
      salva_fornecedor(fi, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_fornecedor(), SEEK_SET);
      fi = le_fornecedor(arq);
    }
    fseek(arq, (i)*tamanho_registro_fornecedor(), SEEK_SET);
    salva_fornecedor(fj, arq);
  }
  fflush(arq);
}

void selection_sort_disco_fornecedor(FILE *arq, int tam) {
    for (int i = 0; i < tam - 1; i++) {
        int min_idx = i;
        fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
        TForn *min_fornecedor = le_fornecedor(arq);

        for (int j = i + 1; j < tam; j++) {
            fseek(arq, j * tamanho_registro_fornecedor(), SEEK_SET);
            TForn *forn_j = le_fornecedor(arq);

            if (forn_j->cod < min_fornecedor->cod) {
                free(min_fornecedor);
                min_fornecedor = forn_j;
                min_idx = j;
            } else {
                free(forn_j);
            }
        }

        if (min_idx != i) {
            fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
            TForn *forn_i = le_fornecedor(arq);

            fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
            salva_fornecedor(min_fornecedor, arq);

            fseek(arq, min_idx * tamanho_registro_fornecedor(), SEEK_SET);
            salva_fornecedor(forn_i, arq);

            free(forn_i);
        }
        free(min_fornecedor);
    }
    fflush(arq);
}