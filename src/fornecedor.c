/**
 * @file fornecedor.c
 * @brief Implementação das funções para manipulação de dados de fornecedores.
 */

#include "fornecedor.h"
#include "log.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

#define NUM_REGISTROS 10;

// Imprime os dados de um fornecedor
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

// Cria uma nova estrutura TForn em memória
TForn *fornecedor(int cod, char *nome, char *cnpj) {
  TForn *forn = (TForn *)malloc(sizeof(TForn));
  if (forn)
    memset(forn, 0, sizeof(TForn));
  forn->cod = cod;
  strcpy(forn->nome, nome);
  strcpy(forn->cnpj, cnpj);

  return forn;
}

// Salva os dados de um fornecedor em um arquivo
void salva_fornecedor(TForn *forn, FILE *out) {
  fwrite(&forn->cod, sizeof(int), 1, out);
  fwrite(forn->nome, sizeof(char), sizeof(forn->nome), out);
  fwrite(forn->cnpj, sizeof(char), sizeof(forn->cnpj), out);
}

// Lê os dados de um fornecedor de um arquivo
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

// Lê e imprime todos os fornecedores de um arquivo
void le_fornecedores(FILE *in) {
  printf("\n\nLendo fornecedores do arquivo...\n\n");
  rewind(in);
  TForn forn;

  while (fread(&forn, sizeof(TForn), 1, in) == 1) {
    imprime_fornecedor(&forn);
  }
}

// Retorna o tamanho da struct TForn em bytes
int tamanho_fornecedor() {
  return sizeof(int)            // cod
         + sizeof(char) * 50    // nome
         + (sizeof(char) * 19); // cnpj
}

// Calcula o número de registros de fornecedores em um arquivo
int tamanho_arquivo_fornecedor(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_fornecedor());
  return tam;
}

// Retorna o tamanho de um registro de fornecedor
int tamanho_registro_fornecedor() { return sizeof(TForn); }

// Gera uma base de dados desordenada de fornecedores
void gerarBaseDesordenada_fornecedor(FILE *file, int numberRecords) {
  int f[numberRecords];
  for (int i = 0; i < numberRecords; i++) {
    f[i] = i + 1;
  }
  embaralhar(f, numberRecords);
  fseek(file, 0, SEEK_SET);

  for (int i = 0; i < numberRecords; i++) {
    TForn forn;
    forn.cod = f[i];
    sprintf(forn.nome, "Fornecedor %d", f[i]);
    sprintf(forn.cnpj, "11.111.111/0001-11");
    fseek(file, (i)*tamanho_registro_fornecedor(), SEEK_SET);
    salva_fornecedor(&forn, file);
  }
}

// Busca um fornecedor por código usando busca sequencial
TForn busca_sequencial_fornecedor(int cod, FILE *arq) {
  int comp = 0;
  TForn forn;
  iniciar_medicao();

  rewind(arq);

  while (fread(&forn, sizeof(TForn), 1, arq) == 1) {
    comp++;
    if (cod == forn.cod) {
      double tempo = finalizar_medicao();
      registrar_tempo_execucao("Busca Sequencial (Fornecedor)", tempo);
      printf("\nComparacoes = %d\n", comp);
      return forn;
    }
  }
  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Busca Sequencial (Fornecedor) - Nao encontrado", tempo);
  printf("\nComparacoes = %d\n", comp);
  forn.cod = -1; // Indica que não foi encontrado
  return forn;
}

// Busca um fornecedor por código usando busca binária
TForn busca_binaria_fornecedor(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  TForn forn;
  iniciar_medicao();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_fornecedor(), SEEK_SET);
    fread(&forn, sizeof(TForn), 1, arq);
    comp++;
    if (cod == forn.cod) {
      double tempo = finalizar_medicao();
      registrar_tempo_execucao("Busca Binaria (Fornecedor)", tempo);
      printf("\nComparacoes = %d\n", comp);
      return forn;
    } else if (forn.cod < cod) {
      left = middle + 1;
    } else {
      right = middle - 1;
    }
  }

  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Busca Binaria (Fornecedor) - Nao encontrado", tempo);
  printf("\nComparacoes = %d\n", comp);
  forn.cod = -1; // Indica que não foi encontrado
  return forn;
}

// Ordena um arquivo de fornecedores usando o método Insertion Sort em disco
void insertion_sort_disco_fornecedor(FILE *arq, int tam) {
  int i;
  iniciar_medicao();
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
      if (i > 0) {
          fseek(arq, (i - 1) * tamanho_registro_fornecedor(), SEEK_SET);
          free(fi);
          fi = le_fornecedor(arq);
      }
    }
    fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
    salva_fornecedor(fj, arq);
    free(fi);
    free(fj);
  }
  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Insertion Sort Disco (Fornecedor)", tempo);
  fflush(arq);
}

// Ordena um arquivo de fornecedores usando o método Selection Sort em disco
void selection_sort_disco_fornecedor(FILE *arq, int tam) {
    iniciar_medicao();
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
    double tempo = finalizar_medicao();
    registrar_tempo_execucao("Selection Sort Disco (Fornecedor)", tempo);
    fflush(arq);
}