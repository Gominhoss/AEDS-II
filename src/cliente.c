/**
 * @file cliente.c
 * @brief Implementação das funções para manipulação de dados de clientes.
 */

#include "cliente.h"
#include "log.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_REGISTROS 10;

// Imprime os dados de um cliente
void imprime_cliente(TCliente *cliente) {
  printf("\n**********************************************");
  printf("\nCliente de codigo ");
  printf("%d", cliente->cod);
  printf("\nNome: ");
  printf("%s", cliente->nome);
  printf("\nCPF: ");
  printf("%s", cliente->cpf);
  printf("\nData de Nascimento: ");
  printf("%s", cliente->data_nascimento);
  printf("\nContato: ");
  printf("%s", cliente->contato);
  printf("\n**********************************************");
}

// Cria uma nova estrutura TCliente em memória
TCliente *cliente(int cod, char *nome, char *cpf, char *data_nascimento,
                  char *contato) {
  TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));
  if (cliente)
    memset(cliente, 0, sizeof(TCliente));
  cliente->cod = cod;
  strcpy(cliente->nome, nome);
  strcpy(cliente->cpf, cpf);
  strcpy(cliente->data_nascimento, data_nascimento);
  strcpy(cliente->contato, contato);
  return cliente;
}

// Salva os dados de um cliente em um arquivo
void salva_cliente(TCliente *cliente, FILE *out) {
  fwrite(&cliente->cod, sizeof(int), 1, out);
  fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
  fwrite(cliente->cpf, sizeof(char), sizeof(cliente->cpf), out);
  fwrite(cliente->data_nascimento, sizeof(char),
         sizeof(cliente->data_nascimento), out);
  fwrite(cliente->contato, sizeof(char), sizeof(cliente->contato), out);
}

// Lê os dados de um cliente de um arquivo
TCliente *le_cliente(FILE *in) {
  TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));
  if (0 >= fread(&cliente->cod, sizeof(int), 1, in)) {
    free(cliente);
    return NULL;
  }
  fread(cliente->nome, sizeof(char), sizeof(cliente->nome), in);
  fread(cliente->cpf, sizeof(char), sizeof(cliente->cpf), in);
  fread(cliente->data_nascimento, sizeof(char),
        sizeof(cliente->data_nascimento), in);
  fread(cliente->contato, sizeof(char), sizeof(cliente->contato), in);
  return cliente;
}

// Lê e imprime todos os clientes de um arquivo
void le_clientes(FILE *in) {
  printf("\n\nLendo clientes do arquivo...\n\n");
  rewind(in);
  TCliente *c = (TCliente *)malloc(sizeof(TCliente));
  while ((c = le_cliente(in)) != NULL) {
    imprime_cliente(c);
    free(c);
  }
}

// Retorna o tamanho da struct TCliente em bytes
int tamanho_cliente() {
  return sizeof(int)          // cod
         + sizeof(char) * 50  // nome
         + sizeof(char) * 15  // cpf
         + sizeof(char) * 11  // data_nascimento
         + sizeof(char) * 20; // contato
}

// Calcula o número de registros de clientes em um arquivo
int tamanho_arquivo_cliente(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_cliente());
  return tam;
}

// Retorna o tamanho de um registro de cliente
int tamanho_registro_cliente() { return sizeof(TCliente); }

// Embaralha os registros em um arquivo de clientes
void embaralhar_arquivo_cliente(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
        TCliente *reg_i = le_cliente(arq);

        fseek(arq, j * tamanho_registro_cliente(), SEEK_SET);
        TCliente *reg_j = le_cliente(arq);

        fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
        salva_cliente(reg_j, arq);

        fseek(arq, j * tamanho_registro_cliente(), SEEK_SET);
        salva_cliente(reg_i, arq);

        free(reg_i);
        free(reg_j);
    }
    fflush(arq);
}

// Gera uma base de dados desordenada de clientes
void gerarBaseDesordenada_cliente(FILE *file, int numberRecords) {
  fseek(file, 0, SEEK_SET);
  for (int i = 0; i < numberRecords; i++) {
      TCliente cli;
      cli.cod = i + 1;
      sprintf(cli.nome, "Cliente %d", cli.cod);
      sprintf(cli.cpf, "111.111.111-11");
      sprintf(cli.data_nascimento, "01/01/2000");
      sprintf(cli.contato, "cliente%d@email.com", cli.cod);
      salva_cliente(&cli, file);
  }
  fflush(file);

  embaralhar_arquivo_cliente(file, numberRecords);
}

// Busca um cliente por código usando busca sequencial
TCliente *busca_sequencial_cliente(int cod, FILE *arq) {
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);

  int comp = 0;
  int i = 0;

  QueryPerformanceCounter(&start);

  for (i = 0; i < tamanho_arquivo_cliente(arq); i++) {
    fseek(arq, i * tamanho_cliente(), SEEK_SET);
    TCliente *cli = le_cliente(arq);
    if (cod == cli->cod) {
      QueryPerformanceCounter(&end);
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return cli;
    } else {
      comp++;
    }
  }

  return NULL;
}

// Busca um cliente por código usando busca binária
TCliente *busca_binaria_cliente(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);
  

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_cliente(), SEEK_SET);
    TCliente *cli = le_cliente(arq);
    if (cod == cli->cod) {
      QueryPerformanceCounter(&end);
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca binaria = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return cli;
    } else if (cli->cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  return NULL;
}

// Ordena um arquivo de clientes usando o método Insertion Sort em disco
void insertion_sort_disco_cliente(FILE *arq, int tam) {
  int i;
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);
  
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_cliente(), SEEK_SET);
    TCliente *cli = le_cliente(arq);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_cliente(), SEEK_SET);
    TCliente *cli1 = le_cliente(arq);
    while ((i > 0) && (cli1->cod > cli->cod)) {
      fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
      salva_cliente(cli1, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_cliente(), SEEK_SET);
      cli1 = le_cliente(arq);
    }
    fseek(arq, (i)*tamanho_registro_cliente(), SEEK_SET);
    salva_cliente(cli, arq);
  }

  QueryPerformanceCounter(&end);
  tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
  registrar_tempo_execucao("Insertion Sort Disco (Cliente)", tempoTotal);
  fflush(arq);
}

// Ordena um arquivo de clientes usando o método Selection Sort em disco
void selection_sort_disco_cliente(FILE *arq, int tam) {
    iniciar_medicao();
    for (int i = 0; i < tam - 1; i++) {
        int min_idx = i;
        fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
        TCliente *cli_min = le_cliente(arq);

        for (int j = i + 1; j < tam; j++) {
            fseek(arq, j * tamanho_registro_cliente(), SEEK_SET);
            TCliente *cli_j = le_cliente(arq);

            if (cli_j->cod < cli_min->cod) {
                free(cli_min);
                cli_min = cli_j;
                min_idx = j;
            } else {
                free(cli_j);
            }
        }

        if (min_idx != i) {
            fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
            TCliente *cli_i = le_cliente(arq);

            fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
            salva_cliente(cli_min, arq);

            fseek(arq, min_idx * tamanho_registro_cliente(), SEEK_SET);
            salva_cliente(cli_i, arq);

            free(cli_i);
        }

        free(cli_min);
    }
    double tempo = finalizar_medicao();
    registrar_tempo_execucao("Selection Sort Disco (Cliente)", tempo);
    fflush(arq);
}