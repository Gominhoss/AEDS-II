/**
 * @file produto.c
 * @brief Implementação das funções para manipulação de dados de produtos.
 */

#include "produto.h"
#include "log.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_REGISTROS 10;

// Imprime os dados de um produto
void imprime_produto(TProd *prod) {
  printf("**********************************************");
  printf("\nProduto de codigo ");
  printf("%d", prod->cod);
  printf("\nNome: ");
  printf("%s", prod->nome);
  printf("\nValor: R$");
  printf("%s", prod->valor);
  printf("\nFornecedor: ");
  printf("%d", prod->cod_forn);
  printf("\n**********************************************");
}

// Cria uma nova estrutura TProd em memória
TProd *produto(int cod, char *nome, char *valor, int cod_forn) {
  TProd *prod = (TProd *)malloc(sizeof(TProd));
  if (prod)
    memset(prod, 0, sizeof(TProd));
  prod->cod = cod;
  strcpy(prod->nome, nome);
  strcpy(prod->valor, valor);
  prod->cod_forn = cod_forn;
  return prod;
}

// Salva os dados de um produto em um arquivo
void salva_produto(TProd *prod, FILE *out) {
  fwrite(&prod->cod, sizeof(int), 1, out);
  fwrite(prod->nome, sizeof(char), sizeof(prod->nome), out);
  fwrite(prod->valor, sizeof(char), sizeof(prod->valor), out);
  fwrite(&prod->cod_forn, sizeof(int), 1, out);
}

// Lê os dados de um produto de um arquivo
TProd *le_produto(FILE *in) {
  TProd *prod = (TProd *)malloc(sizeof(TProd));
  if (0 >= fread(&prod->cod, sizeof(int), 1, in)) {
    free(prod);
    return NULL;
  }
  fread(prod->nome, sizeof(char), sizeof(prod->nome), in);
  fread(prod->valor, sizeof(char), sizeof(prod->valor), in);
  fread(&prod->cod_forn, sizeof(int), 1, in);
  return prod;
}

// Lê e imprime todos os produtos de um arquivo
void le_produtos(FILE *in) {
  printf("\n\nLendo produtos do arquivo...\n\n");
  rewind(in);
  TProd prod;

  while (fread(&prod, sizeof(TProd), 1, in) == 1) {
    imprime_produto(&prod);
  }
}

// Retorna o tamanho da struct TProd em bytes
int tamanho_produto() {
  return sizeof(int)           // cod
         + sizeof(char) * 50   // nome
         + sizeof(char) * 50   // valor
         + sizeof(int);        // cod_forn
}

// Calcula o número de registros de produtos em um arquivo
int tamanho_arquivo_produto(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_produto());
  return tam;
}

// Retorna o tamanho de um registro de produto
int tamanho_registro_produto() { return sizeof(TProd); }

// Embaralha os registros em um arquivo de produtos
void embaralhar_arquivo_produto(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
        TProd *reg_i = le_produto(arq);

        fseek(arq, j * tamanho_registro_produto(), SEEK_SET);
        TProd *reg_j = le_produto(arq);

        fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
        salva_produto(reg_j, arq);

        fseek(arq, j * tamanho_registro_produto(), SEEK_SET);
        salva_produto(reg_i, arq);

        free(reg_i);
        free(reg_j);
    }
    fflush(arq);
}

// Gera uma base de dados desordenada de produtos
void gerarBaseDesordenada_produto(FILE *file, int numberRecords) {
  fseek(file, 0, SEEK_SET);
  for (int i = 0; i < numberRecords; i++) {
      TProd prod;
      prod.cod = i + 1;
      prod.cod_forn = (rand() % 50) + 1;
      sprintf(prod.nome, "Produto %d", prod.cod);
      sprintf(prod.valor, "%.2f", (2.5 * (i + 1))); 
      salva_produto(&prod, file);
  }
  fflush(file);
  embaralhar_arquivo_produto(file, numberRecords);
}

// Busca um produto por código usando busca sequencial
TProd busca_sequencial_produto(int cod, FILE *arq) {
  int comp = 0;
  TProd prod;
  iniciar_medicao();

  rewind(arq);

  while (fread(&prod, sizeof(TProd), 1, arq) == 1) {
    comp++;
    if (cod == prod.cod) {
      double tempo = finalizar_medicao();
      registrar_tempo_execucao("Busca Sequencial (Produto)", tempo);
      printf("\nComparacoes = %d\n", comp);
      return prod;
    }
  }

  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Busca Sequencial (Produto) - Nao encontrado", tempo);
  printf("\nComparacoes = %d\n", comp);
  prod.cod = -1; // Indica que não foi encontrado
  return prod;
}

// Busca um produto por código usando busca binária
TProd busca_binaria_produto(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  TProd prod;
  iniciar_medicao();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_produto(), SEEK_SET);
    fread(&prod, sizeof(TProd), 1, arq);
    comp++;
    if (cod == prod.cod) {
      double tempo = finalizar_medicao();
      registrar_tempo_execucao("Busca Binaria (Produto)", tempo);
      printf("\nComparacoes = %d\n", comp);
      return prod;
    } else if (prod.cod < cod) {
      left = middle + 1;
    } else {
      right = middle - 1;
    }
  }

  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Busca Binaria (Produto) - Nao encontrado", tempo);
  printf("\nComparacoes = %d\n", comp);
  prod.cod = -1; // Indica que não foi encontrado
  return prod;
}

// Ordena um arquivo de produtos usando o método Insertion Sort em disco
void insertion_sort_disco_produto(FILE *arq, int tam) {
  int i;
  iniciar_medicao();
  for (int j = 1; j < tam; j++) {
    fseek(arq, j * tamanho_registro_produto(), SEEK_SET);
    TProd *fj = le_produto(arq);
    i = j - 1;

    fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
    TProd *fi = le_produto(arq);

    while (i >= 0 && fi->cod > fj->cod) {
      fseek(arq, (i + 1) * tamanho_registro_produto(), SEEK_SET);
      salva_produto(fi, arq);
      i--;
      if (i >= 0) {
          fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
          free(fi);
          fi = le_produto(arq);
      }
    }
    fseek(arq, (i + 1) * tamanho_registro_produto(), SEEK_SET);
    salva_produto(fj, arq);
    free(fi);
    free(fj);
  }
  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Insertion Sort Disco (Produto)", tempo);
  fflush(arq);
}

// Ordena um arquivo de produtos usando o método Selection Sort em disco
void selection_sort_disco_produto(FILE *arq, int tam) {
    iniciar_medicao();
    
    for (int i = 0; i < tam - 1; i++) {
        int min_idx = i;
        fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
        TProd *min_produto = le_produto(arq);

        for (int j = i + 1; j < tam; j++) {
            fseek(arq, j * tamanho_registro_produto(), SEEK_SET);
            TProd *prod_j = le_produto(arq);

            if (prod_j->cod < min_produto->cod) {
                free(min_produto);
                min_produto = prod_j;
                min_idx = j;
            } else {
                free(prod_j);
            }
        }

        if (min_idx != i) {
            fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
            TProd *prod_i = le_produto(arq);

            fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
            salva_produto(min_produto, arq);

            fseek(arq, min_idx * tamanho_registro_produto(), SEEK_SET);
            salva_produto(prod_i, arq);

            free(prod_i);
        }
        free(min_produto);
    }

    double tempoTotal = finalizar_medicao();
    registrar_tempo_execucao("SelectionSort (Produto)", tempoTotal);
    fflush(arq);
}
