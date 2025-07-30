#include "cliente.h"
#include "particoes.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_REGISTROS 10;

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

void salva_cliente(TCliente *cliente, FILE *out) {
  fwrite(&cliente->cod, sizeof(int), 1, out);
  fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
  fwrite(cliente->cpf, sizeof(char), sizeof(cliente->cpf), out);
  fwrite(cliente->data_nascimento, sizeof(char),
         sizeof(cliente->data_nascimento), out);
  fwrite(cliente->contato, sizeof(char), sizeof(cliente->contato), out);
}

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

void le_clientes(FILE *in) {
  printf("\n\nLendo clientes do arquivo...\n\n");
  rewind(in);
  TCliente *c = (TCliente *)malloc(sizeof(TCliente));
  while ((c = le_cliente(in)) != NULL) {
    imprime_cliente(c);
    free(c);
  }
}

int tamanho_cliente() {
  return sizeof(int)          // cod
         + sizeof(char) * 50  // nome
         + sizeof(char) * 15  // cpf
         + sizeof(char) * 11  // data_nascimento
         + sizeof(char) * 20; // contato
}

int tamanho_arquivo_cliente(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_cliente());
  return tam;
}

int tamanho_registro_cliente() { return sizeof(TCliente); }

void embaralhar_arquivo_cliente(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        // Troca os registros nas posições i e j
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
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return cli;
      break;
    } else {
      comp++;
    }
  }

  return NULL;
}

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
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
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

void insertion_sort_disco_cliente(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_cliente(), SEEK_SET);
    TCliente *cli = le_cliente(arq);
    // printf("\n********* Funcionario atual: %d\n", fj->cod);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_cliente(), SEEK_SET);
    TCliente *cli1 = le_cliente(arq);
    // printf("fi = %d\n", fi->cod);
    while ((i > 0) && (cli1->cod > cli->cod)) {
      fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
      // printf("Salvando funcionario %d na posicao %d\n", fi->cod, i+1);
      salva_cliente(cli1, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_cliente(), SEEK_SET);
      cli1 = le_cliente(arq);
      // printf("fi = %d; i = %d\n", fi->cod, i);
    }
    fseek(arq, (i)*tamanho_registro_cliente(), SEEK_SET);
    // printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i+1);
    salva_cliente(cli, arq);
  }
  fflush(arq);
}

void selection_sort_disco_cliente(FILE *arq, int tam) {
    for (int i = 0; i < tam - 1; i++) {
        int min_idx = i;
        // lê o registro na posição i (base 0)
        fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
        TCliente *cli_min = le_cliente(arq);

        for (int j = i + 1; j < tam; j++) {
            fseek(arq, j * tamanho_registro_cliente(), SEEK_SET);
            TCliente *cli_j = le_cliente(arq);

            if (cli_j->cod < cli_min->cod) {
                // libera o anterior e atualiza o mínimo
                free(cli_min);
                cli_min = cli_j;
                min_idx = j;
            } else {
                free(cli_j);
            }
        }

        if (min_idx != i) {
            // troca os registros nas posições i e min_idx
            fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
            TCliente *cli_i = le_cliente(arq);

            // grava o menor na posição i
            fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
            salva_cliente(cli_min, arq);

            // grava o que estava na posição i na posição min_idx
            fseek(arq, min_idx * tamanho_registro_cliente(), SEEK_SET);
            salva_cliente(cli_i, arq);

            free(cli_i);
        }

        free(cli_min);
    }
    fflush(arq);
}

void imprime_cod_cli(FILE *in){
    printf("\n\nLendo codigo cliente da particao...\n");
    rewind(in);
    TCliente *f = (TCliente *)malloc(sizeof(TCliente));
    while ((f = le_cliente(in)) != NULL) {
        printf("\nCliente de codigo ");
        printf("%d", f->cod);
        free(f);
    }
}

int classificacaoSubs_cli(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int nFunc = tamanho_arquivo_cliente(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TCliente *v[tamVet];
    TCliente *menor;
    int congela[nFunc];
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * tamanho_cliente(), SEEK_SET);
      v[i] = le_cliente(arq);
      i++;
    }

    i = 0;

    while (i < tamVet){
      congela[i] =0;
      i++;
    }
    i = 0;

    while(proxArq < nFunc || aux < nFunc){
        while (i < tamVet){
            if(congela[i] != 0){
                auxCong++;
            }
            i++;
        }
        i = 0;
        if(proxArq == 5 || auxCong != 0){
            //Cria partição
            sprintf(nome, "particoesCli/particao%d", qtdParticoes);
            char* fim = ".dat";
            strcat(nome, fim);
            tamPart = 0;

            if ((p = fopen(nome, "wb+")) == NULL) {
                printf("Erro criar arquivo de saida\n");
            }
        }

        auxCong = 0;
        while (i < tamVet){
            congela[i] = -1;
            i++;
        }

        while((auxCong < tamVet && proxArq < nFunc) || (auxCong < tamVet && aux < nFunc)){
            aux++;
            menor->cod = INT_MAX;
            posiMenor = nFunc-1;
            for (int j = 0; j < tamVet; j++) {
                if (v[j]->cod < menor->cod && congela[j] == -1 && v[j]->cod != -1) {
                    menor = v[j];
                    posiMenor = j;
                }
            }

            //salva o menor elemento na partição
            fseek(p, (tamPart) * tamanho_cliente(), SEEK_SET);
            salva_cliente(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * tamanho_cliente(), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_cliente(arq);

                if (v[posiMenor]->cod < menor->cod){
                    //verifica se é menor e poe no reservatio
                    congela[posiMenor] = posiMenor;
                    auxCong++;
                }
            } else {
                congela[posiMenor] = posiMenor;
                auxCong++;
                v[posiMenor]->cod = -1;
            }
            proxArq++;

            if(auxCong == tamVet){
                qtdParticoes++;
            }

        }
        imprime_cod_cli(p);
        fclose(p);
    }
    fclose(p);

    return qtdParticoes;
}

int compara_clientes(TCliente *c1, TCliente *c2) {
    if (c1->cod < c2->cod) return -1;
    if (c1->cod > c2->cod) return 1;
    return 0;
}

void intercalacao_clientes_vencedores(char *nome_arquivo_saida, int num_p) {
    FILE *out;
    TVetClientes **v = malloc(sizeof(TVetClientes *) * num_p); // Vetor de ponteiros para as partições
    int i;
    int tam_reg = tamanho_registro_cliente();

    // Aloca e inicializa as partições
    for (i = 0; i < num_p; i++) {
        v[i] = malloc(sizeof(TVetClientes));
        v[i]->particao = malloc(sizeof(TParticao));
        v[i]->cli = malloc(sizeof(TCliente));
        char nome[20];
        sprintf(nome, "cliente%d.dat", i);
        v[i]->f = fopen(nome, "rb");
        v[i]->aux_p = 0;
        v[i]->cli = le_cliente(v[i]->f);
        if (!v[i]->cli) {
            v[i]->cli = cliente(INT_MAX, "NULL", "000.000.000-00", "00/00/0000", "null@null.com");
        }
    }

    // Abre o arquivo de saída
    out = fopen(nome_arquivo_saida, "wb+");
    if (!out) {
        printf("Erro ao criar arquivo de saída.\n");
        return;
    }

    // Estrutura da árvore de vencedores
    TVencedor *arvore = malloc(sizeof(TVencedor) * num_p);

    // Inicializa a árvore com os primeiros registros de cada partição
    for (i = 0; i < num_p; i++) {
        arvore[i].index = i;
        arvore[i].cliente = v[i]->cli;
    }

    // Ordena os nós iniciais da árvore
    for (i = 0; i < num_p - 1; i++) {
        for (int j = i + 1; j < num_p; j++) {
            if (compara_clientes(arvore[i].cliente, arvore[j].cliente) > 0) {
                TVencedor tmp = arvore[i];
                arvore[i] = arvore[j];
                arvore[j] = tmp;
            }
        }
    }

    // Processo de intercalação
    int fim = 0;
    int aux = 0;
    while (!fim) {
        TCliente *menor = arvore[0].cliente;      // Pega o menor cliente na raiz
        int ind_vencedor = arvore[0].index;       // Índice da partição vencedora

        if (menor->cod == INT_MAX) {              // Se o menor for um cliente "sentinela", fim da intercalação
            fim = 1;
            break;
        }

        fseek(out, aux * tam_reg, SEEK_SET);      // Posiciona o cursor no arquivo de saída
        salva_cliente(menor, out);                // Escreve o cliente no arquivo de saída
        aux++;

        // Lê o próximo cliente da partição vencedora
        fseek(v[ind_vencedor]->f, ++v[ind_vencedor]->aux_p * tam_reg, SEEK_SET);
        TCliente *prox = le_cliente(v[ind_vencedor]->f);

        // Se a leitura falhar, cria um cliente sentinela
        if (!prox) {
            prox = cliente(INT_MAX, "NULL", "000.000.000-00", "00/00/0000", "null@null.com");
        }

        v[ind_vencedor]->cli = prox;
        arvore[0].cliente = prox;

        // Atualiza a árvore (reordena os clientes)
        for (i = 0; i < num_p - 1; i++) {
            for (int j = i + 1; j < num_p; j++) {
                if (compara_clientes(arvore[i].cliente, arvore[j].cliente) > 0) {
                    TVencedor tmp = arvore[i];
                    arvore[i] = arvore[j];
                    arvore[j] = tmp;
                }
            }
        }
    }

    // Liberação de memória
    for (i = 0; i < num_p; i++) {
        fclose(v[i]->f);
        free(v[i]->particao);
        free(v[i]->cli);
        free(v[i]);
    }
    free(v);
    free(arvore);
    fclose(out);
}