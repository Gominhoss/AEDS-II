#include "funcionario.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Imprime funcionario
void imprime_funcionario(TFunc *func) {
  printf("\n**********************************************");
  printf("\nFuncionario de codigo ");
  printf("%d", func->cod);
  printf("\nNome: ");
  printf("%s", func->nome);
  printf("\nCPF: ");
  printf("%s", func->cpf);
  printf("\nData de Nascimento: ");
  printf("%s", func->data_nascimento);
  printf("\nSalario: ");
  printf("%.2f", func->salario);
  printf("\n**********************************************");
}

// Cria funcionario. Lembrar de usar free(func)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento,
                   double salario) {
  TFunc *func = (TFunc *)malloc(sizeof(TFunc));
  // inicializa espaço de memória com ZEROS
  if (func)
    memset(func, 0, sizeof(TFunc));
  // copia valores para os campos de func
  func->cod = cod;
  strcpy(func->nome, nome);
  strcpy(func->cpf, cpf);
  strcpy(func->data_nascimento, data_nascimento);
  func->salario = salario;
  return func;
}

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva_funcionario(TFunc *func, FILE *out) {
  fwrite(&func->cod, sizeof(int), 1, out);
  // func->nome ao invés de &func->nome, pois string já é ponteiro
  fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
  fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
  fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento),
         out);
  fwrite(&func->salario, sizeof(double), 1, out);
}

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le_funcionario(FILE *in) {
  TFunc *func = (TFunc *)malloc(sizeof(TFunc));
  if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
    free(func);
    return NULL;
  }
  fread(func->nome, sizeof(char), sizeof(func->nome), in);
  fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
  fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
  fread(&func->salario, sizeof(double), 1, in);
  return func;
}

void le_funcionarios(FILE *in) {
  printf("\n\nLendo funcionarios do arquivo...\n\n");
  rewind(in);
  TFunc *f = (TFunc *)malloc(sizeof(TFunc));
  while ((f = le_funcionario(in)) != NULL) {
    imprime_funcionario(f);
    free(f);
  }
}

// Retorna tamanho do funcionario em bytes
int tamanho_funcionario() {
  return sizeof(int)         // cod
         + sizeof(char) * 50 // nome
         + sizeof(char) * 15 // cpf
         + sizeof(char) * 11 // data_nascimento
         + sizeof(double);   // salario
}

int tamanho_arquivo_funcionario(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_funcionario());
  return tam;
}

int tamanho_registro_funcionario() { return sizeof(TFunc); }

void gerarBaseDesordenada_funcionario(FILE *file, int numberRecords) {  
  for (int i = 0; i < numberRecords; i++) {
        TFunc func;
        func.cod = i + 1;
        sprintf(func.nome, "Funcionario %d", func.cod);
        sprintf(func.cpf, "111.111.111-11");
        sprintf(func.data_nascimento, "01/01/2000");
        func.salario = 1000.2 * i;
        salva_funcionario(&func, file);
  }

  embaralhar(file, numberRecords);
}

TFunc *busca_sequencial_funcionario(int cod, FILE *arq) {
  double tempoTotal = 0;
  int comp = 0;
  int i = 0;

  clock_t inicio = clock();
  for (i = 0; i < tamanho_arquivo_funcionario(arq); i++) {
    fseek(arq, i * tamanho_funcionario(), SEEK_SET);
    TFunc *func = le_funcionario(arq);
    if (cod == func->cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return func;
      break;
    } else {
      comp++;
    }
  }
}

TFunc *busca_binaria_funcionario(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  double tempoTotal = 0;

  time_t inicio = time(NULL);

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *func = le_funcionario(arq);
    if (cod == func->cod) {
      time_t fim = time(NULL);
      tempoTotal = (long)(fim - inicio);
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return func;
    } else if (func->cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  return NULL;
}

void insertion_sort_disco_funcionario(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *fj = le_funcionario(arq);
    // printf("\n********* Funcionario atual: %d\n", fj->cod);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *fi = le_funcionario(arq);
    // printf("fi = %d\n", fi->cod);
    while ((i > 0) && (fi->cod > fj->cod)) {
      fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
      // printf("Salvando funcionario %d na posicao %d\n", fi->cod, i+1);
      salva_funcionario(fi, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_funcionario(), SEEK_SET);
      fi = le_funcionario(arq);
      // printf("fi = %d; i = %d\n", fi->cod, i);
    }
    fseek(arq, (i)*tamanho_registro_funcionario(), SEEK_SET);
    // printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i+1);
    salva_funcionario(fj, arq);
  }
  fflush(arq);
}

void selection_sort_disco_funcionario(FILE *arq, int tam) {
    for (int i = 0; i < tam - 1; i++) {
        int min_idx = i;
        fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
        TFunc *min_func = le_funcionario(arq);

        for (int j = i + 1; j < tam; j++) {
            fseek(arq, j * tamanho_registro_funcionario(), SEEK_SET);
            TFunc *func_j = le_funcionario(arq);

            if (func_j->cod < min_func->cod) {
                free(min_func);
                min_func = func_j;
                min_idx = j;
            } else {
                free(func_j);
            }
        }

        if (min_idx != i) {
            fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
            TFunc *func_i = le_funcionario(arq);

            fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
            salva_funcionario(min_func, arq);

            fseek(arq, min_idx * tamanho_registro_funcionario(), SEEK_SET);
            salva_funcionario(func_i, arq);

            free(func_i);
        }
        free(min_func);
    }
    fflush(arq);
}
