/**
 * @file funcionario.c
 * @brief Implementação das funções para manipulação de dados de funcionários.
 */

#include "funcionario.h"
#include "log.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_REGISTROS 10;

// Imprime os dados de um funcionário
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

// Cria uma nova estrutura TFunc em memória
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento,
                   double salario) {
  TFunc *func = (TFunc *)malloc(sizeof(TFunc));
  if (func)
    memset(func, 0, sizeof(TFunc));
  func->cod = cod;
  strcpy(func->nome, nome);
  strcpy(func->cpf, cpf);
  strcpy(func->data_nascimento, data_nascimento);
  func->salario = salario;

  // Inicializa os novos campos para a tabela hash
  func->ocupado = 1; // Sempre nasce como ocupado
  func->prox = -1;   // Por padrão, não aponta para ninguém

  return func;
}

// Salva os dados de um funcionário em um arquivo
void salva_funcionario(TFunc *func, FILE *out) {
  fwrite(&func->cod, sizeof(int), 1, out);
  fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
  fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
  fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento),
         out);
  fwrite(&func->salario, sizeof(double), 1, out);

  // Salva os novos campos para a tabela hash
  fwrite(&func->ocupado, sizeof(int), 1, out);
  fwrite(&func->prox, sizeof(int), 1, out);

}

// Lê os dados de um funcionário de um arquivo
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

// Lê e imprime todos os funcionários de um arquivo
void le_funcionarios(FILE *in) {
  printf("\n\nLendo funcionarios do arquivo...\n\n");
  rewind(in);
  TFunc *f;
  while ((f = le_funcionario(in)) != NULL) {
    imprime_funcionario(f);
    free(f);
  }
}

// Retorna o tamanho da struct TFunc em bytes
int tamanho_funcionario() {
  return sizeof(int)         // cod
         + sizeof(char) * 50 // nome
         + sizeof(char) * 15 // cpf
         + sizeof(char) * 11 // data_nascimento
         + sizeof(double);   // salario
}

// Calcula o número de registros de funcionários em um arquivo
int tamanho_arquivo_funcionario(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_funcionario());
  return tam;
}

// Retorna o tamanho de um registro de funcionário
int tamanho_registro_funcionario() { return sizeof(TFunc); }

// Embaralha os registros em um arquivo de funcionários
void embaralhar_arquivo_funcionario(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
        TFunc *reg_i = le_funcionario(arq);

        fseek(arq, j * tamanho_registro_funcionario(), SEEK_SET);
        TFunc *reg_j = le_funcionario(arq);

        fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
        salva_funcionario(reg_j, arq);

        fseek(arq, j * tamanho_registro_funcionario(), SEEK_SET);
        salva_funcionario(reg_i, arq);

        free(reg_i);
        free(reg_j);
    }
    fflush(arq);
}

// Gera uma base de dados desordenada de funcionários
void gerarBaseDesordenada_funcionario(FILE *file, int numberRecords) {  
  fseek(file, 0, SEEK_SET);
  for (int i = 0; i < numberRecords; i++) {
        TFunc func;
        func.cod = i + 1;
        sprintf(func.nome, "Funcionario %d", func.cod);
        sprintf(func.cpf, "111.111.111-11");
        sprintf(func.data_nascimento, "01/01/2000");
        func.salario = 1000.2 * (i + 1);
        salva_funcionario(&func, file);
  }

  embaralhar_arquivo_funcionario(file, numberRecords);
}

// Busca um funcionário por código usando busca sequencial
TFunc *busca_sequencial_funcionario(int cod, FILE *arq) {
  int comp = 0;
  TFunc *func = NULL;
  iniciar_medicao();

  rewind(arq);
  while((func = le_funcionario(arq)) != NULL){
      comp++;
      if(cod == func->cod){
          double tempo = finalizar_medicao();
          registrar_tempo_execucao("Busca Sequencial (Funcionario)", tempo);
          printf("\nComparacoes = %d\n", comp);
          return func;
      }
      free(func);
  }

  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Busca Sequencial (Funcionario) - Nao encontrado", tempo);
  printf("\nComparacoes = %d\n", comp);
  return NULL;
}

// Busca um funcionário por código usando busca binária
TFunc *busca_binaria_funcionario(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  iniciar_medicao();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *func = le_funcionario(arq);
    comp++;
    if (cod == func->cod) {
      double tempo = finalizar_medicao();
      registrar_tempo_execucao("Busca Binaria (Funcionario)", tempo);
      printf("\nComparacoes = %d\n", comp);
      return func;
    } else if (func->cod < cod) {
      left = middle + 1;
    } else {
      right = middle - 1;
    }
    free(func);
  }

  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Busca Binaria (Funcionario) - Nao encontrado", tempo);
  printf("\nComparacoes = %d\n", comp);
  return NULL;
}

// Ordena um arquivo de funcionários usando o método Insertion Sort em disco
void insertion_sort_disco_funcionario(FILE *arq, int tam) {
  int i;
  iniciar_medicao();
  for (int j = 1; j < tam; j++) {
    fseek(arq, j * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *fj = le_funcionario(arq);
    i = j - 1;

    fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *fi = le_funcionario(arq);

    while (i >= 0 && fi->cod > fj->cod) {
      fseek(arq, (i + 1) * tamanho_registro_funcionario(), SEEK_SET);
      salva_funcionario(fi, arq);
      i--;
      if (i >= 0) {
          fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
          free(fi);
          fi = le_funcionario(arq);
      }
    }
    fseek(arq, (i + 1) * tamanho_registro_funcionario(), SEEK_SET);
    salva_funcionario(fj, arq);
    free(fi);
    free(fj);
  }
  double tempo = finalizar_medicao();
  registrar_tempo_execucao("Insertion Sort Disco (Funcionario)", tempo);
  fflush(arq);
}

// Ordena um arquivo de funcionários usando o método Selection Sort em disco
void selection_sort_disco_funcionario(FILE *arq, int tam) {
  iniciar_medicao();
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

    double tempoTotal = finalizar_medicao();
    registrar_tempo_execucao("SelectionSort (Funcionario)", tempoTotal);
    fflush(arq);
}