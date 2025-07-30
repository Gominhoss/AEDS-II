#include "funcionario.h"
#include "particoes.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_REGISTROS 10;

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

void embaralhar_arquivo_funcionario(FILE *arq, int total_records) {
    if (total_records <= 1) return;

    // Garante que a semente do gerador aleatório é diferente a cada execução
    srand(time(NULL));

    // Itera do primeiro ao penúltimo registro
    for (int i = 0; i < total_records - 1; i++) {
        // 1. Escolhe um índice aleatório 'j' entre 'i' e o final do arquivo
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        // 2. Lê o registro da posição i
        fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
        TFunc *reg_i = le_funcionario(arq);

        // 3. Lê o registro da posição j
        fseek(arq, j * tamanho_registro_funcionario(), SEEK_SET);
        TFunc *reg_j = le_funcionario(arq);

        // 4. Escreve o registro 'reg_j' na posição 'i'
        fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
        salva_funcionario(reg_j, arq);

        // 5. Escreve o registro 'reg_i' na posição 'j'
        fseek(arq, j * tamanho_registro_funcionario(), SEEK_SET);
        salva_funcionario(reg_i, arq);

        // 6. Libera a memória alocada para os registros temporários
        free(reg_i);
        free(reg_j);
    }
    fflush(arq); // Garante que todas as escritas foram para o disco
}

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

  embaralhar_arquivo_funcionario(file, numberRecords);
}

TFunc *busca_sequencial_funcionario(int cod, FILE *arq) {
  int comp = 0;
  int i = 0;

  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);
  for (i = 0; i < tamanho_arquivo_funcionario(arq); i++) {
    fseek(arq, i * tamanho_funcionario(), SEEK_SET);
    TFunc *func = le_funcionario(arq);
    if (cod == func->cod) {
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
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

  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *func = le_funcionario(arq);
    if (cod == func->cod) {
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
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

void imprime_cod_func(FILE *in){
    printf("\n\nLendo codigo funcionario da particao...\n");
    rewind(in);
    TFunc *f = (TFunc *)malloc(sizeof(TFunc));
    while ((f = le_funcionario(in)) != NULL) {
        printf("\nFuncionario de codigo ");
        printf("%d", f->cod);
        free(f);
    }
}

int classificacaoSubs_func(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo
    
    int nFunc = tamanho_arquivo_funcionario(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamVet = 5;
    TFunc *v[tamVet];
    TFunc *menor;
    int congela[nFunc];
    TFunc *f;
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * tamanho_funcionario(), SEEK_SET);
      v[i] = le_funcionario(arq);
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
            sprintf(nome, "particoesFunc/particao%d", qtdParticoes);
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
            fseek(p, (tamPart) * tamanho_funcionario(), SEEK_SET);
            salva_funcionario(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * tamanho_funcionario(), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_funcionario(arq);

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
        imprime_cod_func(p);
        fclose(p);
    }
    fclose(p);

    return qtdParticoes;

}

int compara_funcionarios(const TFunc *f1, const TFunc *f2) {
    if (f1->cod < f2->cod) return -1;
    if (f1->cod > f2->cod) return 1;
    return 0;
}


void intercalacao_funcionarios_vencedores(char *nome_arquivo_saida, int num_p) {
    FILE *out;
    TVetFunc **v = malloc(sizeof(TVetFunc *) * num_p);
    int i;
    int tam_reg = tamanho_registro_funcionario();

    // Aloca e inicializa as partições
    for (i = 0; i < num_p; i++) {
        v[i] = malloc(sizeof(TVetFunc));
        v[i]->particao = malloc(sizeof(TParticao));
        v[i]->func = NULL;
        char nome[20];
        sprintf(nome, "funcionario%d.dat", i);
        v[i]->f = fopen(nome, "rb");
        v[i]->aux_p = 0;
        v[i]->func = le_funcionario(v[i]->f);
        if (!v[i]->func) {
            // Cria funcionário "sentinela"
            v[i]->func = funcionario(INT_MAX, "NULL", "NULL", "00/00/0000", 0.0);
        }
        v[i]->read = 0;
    }

    out = fopen(nome_arquivo_saida, "wb+");
    if (!out) {
        printf("Erro ao criar arquivo de saída.\n");
        for (int k = 0; k < num_p; k++) {
            if (v[k]->f) fclose(v[k]->f);
            if (v[k]->particao) free(v[k]->particao);
            if (v[k]->func) free(v[k]->func);
            free(v[k]);
        }
        free(v);
        return;
    }

    // Cria a árvore de vencedores
    TVencedorFunc *arvore = malloc(sizeof(TVencedorFunc) * num_p);

    for (i = 0; i < num_p; i++) {
        arvore[i].index = i;
        arvore[i].funcionario = v[i]->func;
    }

    // Ordena a árvore inicial
    for (i = 0; i < num_p - 1; i++) {
        for (int j = i + 1; j < num_p; j++) {
            if (compara_funcionarios(arvore[i].funcionario, arvore[j].funcionario) > 0) {
                TVencedorFunc tmp = arvore[i];
                arvore[i] = arvore[j];
                arvore[j] = tmp;
            }
        }
    }

    int fim = 0;
    int aux = 0;
    while (!fim) {
        TFunc *menor = arvore[0].funcionario;
        int ind_vencedor = arvore[0].index;

        if (menor->cod == INT_MAX) {
            fim = 1;
            break;
        }

        fseek(out, aux * tam_reg, SEEK_SET);
        salva_funcionario(menor, out);
        aux++;

        fseek(v[ind_vencedor]->f, ++v[ind_vencedor]->aux_p * tam_reg, SEEK_SET);
        TFunc *prox = le_funcionario(v[ind_vencedor]->f);

        if (!prox) {
            prox = funcionario(INT_MAX, "NULL", "NULL", "00/00/0000", 0.0);
        }

        free(v[ind_vencedor]->func);
        v[ind_vencedor]->func = prox;
        arvore[0].funcionario = prox;

        // Reordena a árvore de vencedores
        for (i = 0; i < num_p - 1; i++) {
            for (int j = i + 1; j < num_p; j++) {
                if (compara_funcionarios(arvore[i].funcionario, arvore[j].funcionario) > 0) {
                    TVencedorFunc tmp = arvore[i];
                    arvore[i] = arvore[j];
                    arvore[j] = tmp;
                }
            }
        }
    }

    // Libera memórias
    for (i = 0; i < num_p; i++) {
        fclose(v[i]->f);
        free(v[i]->particao);
        if (v[i]->func) free(v[i]->func);
        free(v[i]);
    }
    free(v);
    free(arvore);
    fclose(out);
}