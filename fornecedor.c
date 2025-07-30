#include "fornecedor.h"
#include "particoes.h"
#include "log.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

#define NUM_REGISTROS 10;

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

TForn busca_sequencial_fornecedor(int cod, FILE *arq) {
  double tempoTotal = 0;
  int comp = 0;
  int i = 0;
  clock_t inicio = clock();

  rewind(arq);
  TForn forn;

  while (fread(&forn, sizeof(TForn), 1, arq) == 1) {
    if (cod == forn.cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
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
  double tempoTotal = 0;
  TForn forn;

  rewind(arq);
  clock_t inicio = clock();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_fornecedor(), SEEK_SET);
    fread(&forn, sizeof(TForn), 1, arq);
    if (cod == forn.cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
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

void imprime_cod_forn(FILE *in){
    printf("\n\nLendo codigo fornecedor da particao...\n");
    rewind(in);
    TForn forn;

    while (fread(&forn, sizeof(TForn), 1, in) == 1) {
        printf("\nFornecedor de codigo %d", forn.cod);
    }
}

int classificacaoSubs_forn(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int nFunc = tamanho_arquivo_fornecedor(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TForn *v[tamVet];
    TForn *menor;
    int congela[nFunc];
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;
    TForn forn;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * sizeof(TForn), SEEK_SET);
      v[i] = le_fornecedor(arq);
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
            sprintf(nome, "particoesForn/particao%d", qtdParticoes);
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
            fseek(p, (tamPart) * sizeof(TForn), SEEK_SET);
            salva_fornecedor(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * sizeof(TForn), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_fornecedor(arq);

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
        imprime_cod_forn(p);
        fclose(p);
    }
    fclose(p);

    return qtdParticoes;
}

int compara_fornecedores(const TForn *f1, const TForn *f2) {
    if (f1->cod < f2->cod) return -1;
    if (f1->cod > f2->cod) return 1;
    return 0;
}

TForn* fornecedor_sentinela() {
    TForn *f = malloc(sizeof(TForn));
    f->cod = INT_MAX;  // Código máximo para sentinela
    strcpy(f->nome, "NULL");
    strcpy(f->cnpj, "NULL");
    return f;
}


void intercalacao_fornecedores_vencedores(char *nome_arquivo_saida, int num_p) {
    FILE *out;
    TVetForn **v = malloc(sizeof(TVetForn *) * num_p); // vetor de ponteiros para partições
    int i;
    int tam_reg = tamanho_registro_fornecedor();

    // Aloca e inicializa as partições
    for (i = 0; i < num_p; i++) {
        v[i] = malloc(sizeof(TVetForn));
        v[i]->particao = malloc(sizeof(TParticao));
        char nome[20];
        sprintf(nome, "fornecedor%d.dat", i);
        v[i]->f = fopen(nome, "rb");
        v[i]->aux_p = 0;
        v[i]->en = le_fornecedor(v[i]->f);  // lê o primeiro registro da partição
        if (!v[i]->en) {
            v[i]->en = fornecedor_sentinela();  // registro sentinela para fim
        }
    }

    // Abre o arquivo de saída
    out = fopen(nome_arquivo_saida, "wb+");
    if (!out) {
        printf("Erro ao criar arquivo de saída.\n");
        // Libera memória antes de sair
        for (i = 0; i < num_p; i++) {
            fclose(v[i]->f);
            free(v[i]->particao);
            free(v[i]);
        }
        free(v);
        return;
    }

    // Cria e inicializa a árvore de vencedores
    TVencedorForn *arvore = malloc(sizeof(TVencedorForn) * num_p);
    for (i = 0; i < num_p; i++) {
        arvore[i].index = i;
        arvore[i].fornecedor = v[i]->en;
    }

    // Ordena os nós iniciais da árvore (bubble sort simples)
    for (i = 0; i < num_p - 1; i++) {
        for (int j = i + 1; j < num_p; j++) {
            if (compara_fornecedores(arvore[i].fornecedor, arvore[j].fornecedor) > 0) {
                TVencedorForn tmp = arvore[i];
                arvore[i] = arvore[j];
                arvore[j] = tmp;
            }
        }
    }

    int fim = 0;
    int aux = 0;
    while (!fim) {
        TForn *menor = arvore[0].fornecedor;
        int ind_vencedor = arvore[0].index;

        if (menor->cod == INT_MAX) {  // fim da intercalação
            fim = 1;
            break;
        }

        fseek(out, aux * tam_reg, SEEK_SET);
        salva_fornecedor(menor, out);
        aux++;

        // Lê próximo registro da partição vencedora
        fseek(v[ind_vencedor]->f, ++v[ind_vencedor]->aux_p * tam_reg, SEEK_SET);
        TForn *prox = le_fornecedor(v[ind_vencedor]->f);

        if (!prox) {
            prox = fornecedor_sentinela();
        }

        v[ind_vencedor]->en = prox;
        arvore[0].fornecedor = prox;

        // Reordena a árvore
        for (i = 0; i < num_p - 1; i++) {
            for (int j = i + 1; j < num_p; j++) {
                if (compara_fornecedores(arvore[i].fornecedor, arvore[j].fornecedor) > 0) {
                    TVencedorForn tmp = arvore[i];
                    arvore[i] = arvore[j];
                    arvore[j] = tmp;
                }
            }
        }
    }

    // Libera memória
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