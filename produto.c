#include "produto.h"
#include "particoes.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define NUM_REGISTROS 10;

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

void salva_produto(TProd *prod, FILE *out) {
  fwrite(&prod->cod, sizeof(int), 1, out);
  fwrite(prod->nome, sizeof(char), sizeof(prod->nome), out);
  fwrite(prod->valor, sizeof(char), sizeof(prod->valor), out);
  fwrite(&prod->cod_forn, sizeof(int), 1, out); // << NOVO
}


TProd *le_produto(FILE *in) {
  TProd *prod = (TProd *)malloc(sizeof(TProd));
  if (0 >= fread(&prod->cod, sizeof(int), 1, in)) {
    free(prod);
    return NULL;
  }
  fread(prod->nome, sizeof(char), sizeof(prod->nome), in);
  fread(prod->valor, sizeof(char), sizeof(prod->valor), in);
  fread(&prod->cod_forn, sizeof(int), 1, in); // << NOVO
  return prod;
}


void le_produtos(FILE *in) {
  printf("\n\nLendo produtos do arquivo...\n\n");
  rewind(in);
  TProd prod;

  while (fread(&prod, sizeof(TProd), 1, in) == 1) {
    imprime_produto(&prod);
  }
}

int tamanho_produto() {
  return sizeof(int)           // cod
         + sizeof(char) * 50   // nome
         + sizeof(char) * 50   // valor
         + sizeof(int);        // cod_forn << NOVO
}

int tamanho_arquivo_produto(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_produto());
  return tam;
}

int tamanho_registro_produto() { return sizeof(TProd); }

void embaralhar_arquivo_produto(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        // Troca os registros nas posições i e j
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

void gerarBaseDesordenada_produto(FILE *file, int numberRecords) {
  fseek(file, 0, SEEK_SET);
  for (int i = 0; i < numberRecords; i++) {
      TProd prod;
      prod.cod = i + 1;
      prod.cod_forn = (rand() % 50) + 1;
      sprintf(prod.nome, "Produto %d", prod.cod);
      // Corrigido para salvar valor como float/double, não string
      sprintf(prod.valor, "%.2f", (2.5 * (i + 1))); 
      // sprintf(fornecedor_str, "Fornecedor %d", prod.cod_forn);

      salva_produto(&prod, file);
  }
  fflush(file);
  embaralhar_arquivo_produto(file, numberRecords);
}

TProd busca_sequencial_produto(int cod, FILE *arq) {
  int comp = 0;
  int i = 0;
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  rewind(arq);
  TProd prod;

  while (fread(&prod, sizeof(TProd), 1, arq) == 1) {
    if (cod == prod.cod) {
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return prod;
      break;
    } else {
      comp++;
    }
  }

  return prod;
}

TProd busca_binaria_produto(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  TProd prod;

  rewind(arq);
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  double tempoTotal;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_produto(), SEEK_SET);
    fread(&prod, sizeof(TProd), 1, arq);
    if (cod == prod.cod) {
      QueryPerformanceCounter(&end); // Para a contagem
      tempoTotal = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return prod;
    } else if (prod.cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  return prod;
}

void insertion_sort_disco_produto(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_produto(), SEEK_SET);
    TProd *cli = le_produto(arq);
    // printf("\n********* Funcionario atual: %d\n", fj->cod);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_produto(), SEEK_SET);
    TProd *cli1 = le_produto(arq);
    // printf("fi = %d\n", fi->cod);
    while ((i > 0) && (cli1->cod > cli->cod)) {
      fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
      // printf("Salvando funcionario %d na posicao %d\n", fi->cod, i+1);
      salva_produto(cli1, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_produto(), SEEK_SET);
      cli1 = le_produto(arq);
      // printf("fi = %d; i = %d\n", fi->cod, i);
    }
    fseek(arq, (i)*tamanho_registro_produto(), SEEK_SET);
    // printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i+1);
    salva_produto(cli, arq);
  }
  fflush(arq);
}

void selection_sort_disco_produto(FILE *arq, int tam) {
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
    fflush(arq);
}

void imprime_cod_prod(FILE *in){
    printf("\n\nLendo codigo produto da particao...\n");
    rewind(in);
    TProd forn;

    while (fread(&forn, sizeof(TProd), 1, in) == 1) {
        printf("\nProduto de codigo %d", forn.cod);
    }
}


int classificacaoSubs_prod(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int nFunc = tamanho_arquivo_produto(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TProd *v[tamVet];
    TProd *menor;
    int congela[nFunc];
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;
    TProd forn;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * sizeof(TProd), SEEK_SET);
      v[i] = le_produto(arq);
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
            sprintf(nome, "particoesProd/particao%d", qtdParticoes);
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
            fseek(p, (tamPart) * sizeof(TProd), SEEK_SET);
            salva_produto(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * sizeof(TProd), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_produto(arq);

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
        imprime_cod_prod(p);
        fclose(p);
    }
    fclose(p);

    return qtdParticoes;
}

int compara_produtos(TProd *p1, TProd *p2) {
    if (p1->cod < p2->cod) return -1;
    if (p1->cod > p2->cod) return 1;
    return 0;
}

// Função para realizar a intercalação de arquivos de produtos usando árvore de vencedores
void intercalacao_produtos_vencedores(char *nome_arquivo_saida, int num_p) {
    FILE *out;
    TVetProd **v = malloc(sizeof(TVetProd *) * num_p); // Vetor de ponteiros para as partições
    int i;
    int tam_reg = tamanho_registro_produto();

    // Aloca e inicializa as partições
    for (i = 0; i < num_p; i++) {
        v[i] = malloc(sizeof(TVetProd));
        v[i]->particao = malloc(sizeof(TParticao));
        // Corrigido: aloca o ponteiro en, não "prod"
        v[i]->en = NULL;  // inicializa como NULL
        char nome[20];
        sprintf(nome, "produto%d.dat", i);
        v[i]->f = fopen(nome, "rb");
        v[i]->aux_p = 0;
        v[i]->en = le_produto(v[i]->f);
        if (!v[i]->en) {
            // Corrigido: chamada correta da função produto com 4 parâmetros
            v[i]->en = produto(INT_MAX, "NULL", "NULL", 0);
        }
        v[i]->read = 0;  // se necessário inicializar read também
    }

    // Abre o arquivo de saída
    out = fopen(nome_arquivo_saida, "wb+");
    if (!out) {
        printf("Erro ao criar arquivo de saída.\n");
        // Liberar memórias alocadas antes de sair
        for (int k = 0; k < num_p; k++) {
            if (v[k]->f) fclose(v[k]->f);
            if (v[k]->particao) free(v[k]->particao);
            if (v[k]->en) free(v[k]->en);
            free(v[k]);
        }
        free(v);
        return;
    }

    // Estrutura da árvore de vencedores
    TVencedorProduto *arvore = malloc(sizeof(TVencedorProduto) * num_p);

    // Inicializa a árvore com os primeiros registros de cada partição
    for (i = 0; i < num_p; i++) {
        arvore[i].index = i;
        arvore[i].produto = v[i]->en;
    }

    // Ordena os nós iniciais da árvore
    for (i = 0; i < num_p - 1; i++) {
        for (int j = i + 1; j < num_p; j++) {
            if (compara_produtos(arvore[i].produto, arvore[j].produto) > 0) {
                TVencedorProduto tmp = arvore[i];
                arvore[i] = arvore[j];
                arvore[j] = tmp;
            }
        }
    }

    // Processo de intercalação
    int fim = 0;
    int aux = 0;
    while (!fim) {
        TProd *menor = arvore[0].produto;      // Pega o menor produto na raiz
        int ind_vencedor = arvore[0].index;    // Índice da partição vencedora

        if (menor->cod == INT_MAX) {            // Produto sentinela indica fim
            fim = 1;
            break;
        }

        fseek(out, aux * tam_reg, SEEK_SET);    // Posiciona cursor arquivo saída
        salva_produto(menor, out);              // Salva produto no arquivo saída
        aux++;

        // Lê o próximo produto da partição vencedora
        fseek(v[ind_vencedor]->f, ++v[ind_vencedor]->aux_p * tam_reg, SEEK_SET);
        TProd *prox = le_produto(v[ind_vencedor]->f);

        // Se falhar leitura, cria produto sentinela
        if (!prox) {
            prox = produto(INT_MAX, "NULL", "NULL", 0);
        }

        // Atualiza produto na partição e na árvore
        free(v[ind_vencedor]->en); // liberar produto antigo
        v[ind_vencedor]->en = prox;
        arvore[0].produto = prox;

        // Reordena a árvore de vencedores
        for (i = 0; i < num_p - 1; i++) {
            for (int j = i + 1; j < num_p; j++) {
                if (compara_produtos(arvore[i].produto, arvore[j].produto) > 0) {
                    TVencedorProduto tmp = arvore[i];
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
        if (v[i]->en) free(v[i]->en);
        free(v[i]);
    }
    free(v);
    free(arvore);
    fclose(out);
}
