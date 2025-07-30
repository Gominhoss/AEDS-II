#include "estoque.h"
#include "produto.h"
#include "particoes.h"
#include "log.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_REGISTROS 10;

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

void gerarBaseDesordenada_estoque(FILE *fileEstoque, FILE *fileProdutos) {
    srand(time(NULL));

    rewind(fileProdutos);
    fseek(fileEstoque, 0, SEEK_SET);

    TProd *prod;
    while ((prod = le_produto(fileProdutos)) != NULL) {
        TEstoque estoque;
        estoque.cod = prod->cod;             // código do produto
        estoque.atual = rand() % 501;        // estoque atual aleatório até 500
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


TEstoque busca_sequencial_estoque(int cod, FILE *arq) {
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
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

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

    QueryPerformanceCounter(&end); // Para a contagem
    tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    registrar_tempo_execucao("SelectionSort: ", tempoTotal);
    fflush(arq);
}

void imprime_cod_est(FILE *in){
    printf("\n\nLendo codigo estoque da particao...\n");
    rewind(in);
    TEstoque forn;

    while (fread(&forn, sizeof(TEstoque), 1, in) == 1) {
        printf("\nEstoque de codigo %d", forn.cod);
    }
}

int classificacaoSubs_estoq(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int nFunc = tamanho_arquivo_estoque(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TEstoque *v[tamVet];
    TEstoque *menor;
    int congela[nFunc];
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;
    TEstoque forn;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * sizeof(TEstoque), SEEK_SET);
      v[i] = le_estoque(arq);
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
            sprintf(nome, "particoesEstoq/particao%d", qtdParticoes);
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
            fseek(p, (tamPart) * sizeof(TEstoque), SEEK_SET);
            salva_estoque(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * sizeof(TEstoque), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_estoque(arq);

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
        imprime_cod_est(p);
        fclose(p);
    }
    fclose(p);
    
    return qtdParticoes;
}

int compara_estoque(TEstoque *e1, TEstoque *e2) {
    if (e1->cod < e2->cod) return -1;
    if (e1->cod > e2->cod) return 1;
    return 0;
}

// Função para realizar a intercalação de arquivos de estoque usando árvore de vencedores
void intercalacao_estoque_vencedores(char *nome_arquivo_saida, int num_p) {
    FILE *out;
    TVetEstoque **v = malloc(sizeof(TVetEstoque *) * num_p); // Vetor de ponteiros para as partições
    int i;
    int tam_reg = tamanho_registro_estoque(); // Tamanho de um registro de estoque

    // Aloca e inicializa as partições
    for (i = 0; i < num_p; i++) {
        v[i] = malloc(sizeof(TVetEstoque));
        v[i]->particao = malloc(sizeof(TParticao));
        v[i]->en = malloc(sizeof(TEstoque));
        char nome[20];
        sprintf(nome, "estoque%d.dat", i);           // Nome do arquivo da partição
        v[i]->f = fopen(nome, "rb");                 // Abre o arquivo da partição
        v[i]->aux_p = 0;
        v[i]->en = le_estoque(v[i]->f);              // Lê o primeiro registro de estoque
        if (!v[i]->en) {
            v[i]->en = estoque(INT_MAX, 0, 0, 0);     // Cria um registro sentinela
        }
    }

    // Abre o arquivo de saída
    out = fopen(nome_arquivo_saida, "wb+");
    if (!out) {
        printf("Erro ao criar arquivo de saída.\n");
        return;
    }

    // Estrutura da árvore de vencedores
    typedef struct {
        int index;
        TEstoque *estoque;
    } TVencedorEstoque;

    TVencedorEstoque *arvore = malloc(sizeof(TVencedorEstoque) * num_p);

    // Inicializa a árvore com os primeiros registros de cada partição
    for (i = 0; i < num_p; i++) {
        arvore[i].index = i;
        arvore[i].estoque = v[i]->en;
    }

    // Ordena os nós iniciais da árvore
    for (i = 0; i < num_p - 1; i++) {
        for (int j = i + 1; j < num_p; j++) {
            if (compara_estoque(arvore[i].estoque, arvore[j].estoque) > 0) {
                TVencedorEstoque tmp = arvore[i];
                arvore[i] = arvore[j];
                arvore[j] = tmp;
            }
        }
    }

    // Processo de intercalação
    int fim = 0;
    int aux = 0;
    while (!fim) {
        TEstoque *menor = arvore[0].estoque;          // Pega o menor estoque na raiz
        int ind_vencedor = arvore[0].index;           // Índice da partição vencedora

        if (menor->cod == INT_MAX) {                  // Se o menor for sentinela, fim da intercalação
            fim = 1;
            break;
        }

        fseek(out, aux * tam_reg, SEEK_SET);          // Posiciona o cursor no arquivo de saída
        salva_estoque(menor, out);                    // Escreve o estoque no arquivo de saída
        aux++;

        // Lê o próximo estoque da partição vencedora
        fseek(v[ind_vencedor]->f, ++v[ind_vencedor]->aux_p * tam_reg, SEEK_SET);
        TEstoque *prox = le_estoque(v[ind_vencedor]->f);

        // Se a leitura falhar, cria um estoque sentinela
        if (!prox) {
            prox = estoque(INT_MAX, 0, 0, 0);
        }

        v[ind_vencedor]->en = prox;
        arvore[0].estoque = prox;

        // Atualiza a árvore (reordena os estoques)
        for (i = 0; i < num_p - 1; i++) {
            for (int j = i + 1; j < num_p; j++) {
                if (compara_estoque(arvore[i].estoque, arvore[j].estoque) > 0) {
                    TVencedorEstoque tmp = arvore[i];
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
        free(v[i]->en);
        free(v[i]);
    }
    free(v);
    free(arvore);
    fclose(out);
}
