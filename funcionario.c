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


void classificacaoSubs_func(FILE *arq) {
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
}

// Função para mesclar duas metades do array em ordem crescente
void merge_funcionarios(TVetFunc **arr, int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  // Criando arrays temporários para armazenar as duas metades
  TVetFunc **L = (TVetFunc **)malloc(n1 * sizeof(TVetFunc *));
  TVetFunc **R = (TVetFunc **)malloc(n2 * sizeof(TVetFunc *));

  // Copiando os dados para os arrays temporários L e R
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  // Mesclando os arrays temporários de volta para arr[l..r]
  i = 0;
  j = 0;
  k = l;

  while (i < n1 && j < n2) {
    if (L[i]->func-> cod <= R[j]->func ->cod) {

      arr[k] = L[i];
      i++;

    } else {

      arr[k] = R[j];
      j++;
    }

    k++;
  }

  // Copiando os elementos restantes de L e R, se houver
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }

  // Liberando memória dos arrays temporários
  free(L);
  free(R);

  // printf("\nMerge Finalizado.\n");
}

// Função principal do Merge Sort
void mergeSort_funcionarios(TVetFunc **arr, int l, int r) {
  // printf("\nEntrou no MergeSort.\n");
  if (l < r) {
    int m = l + (r - l) / 2;

    // Ordenando a primeira e a segunda metade do array
    mergeSort_funcionarios(arr, l, m);
    mergeSort_funcionarios(arr, m + 1, r);

    // Mesclando as duas metades ordenadas
    merge_funcionarios(arr, l, m, r);
  }
}

void processa_particoes_funcionarios(TVetFunc **v, TNomes *nome_particoes, int num_p, char *out) {
  int index = 0;
  int fim = 0;
  int read_count = 0;
  int last;
  int j = 1;

  printf("\nCheckpoint 1: entrou na funcao processar\n");

  for (int i = 0; i < num_p; i += 2) { // inicia intercalando em pares os vetores/particoes
    if ((v[j]) == NULL || j >= num_p) {
      break;
    }
    intercala_particoes_funcionarios(v[i], v[j], nome_particoes, (num_p + index));
    index++;
    j += 2;
  }

   printf("\nCheckpoint 4: saiu da intercalacao inicial do processar\n");

    int flag1 = 0;
    int flag2 = 0;
    char *p_nome;
    int count_sentinel = 0;

  while (!fim) { // nessa segunda iteração, irá verificar e intercalar os vetores que não tiverem sinalizados de "read" (lidos)
    TVetFunc *pt1 = NULL;
    TVetFunc *pt2 = NULL;
    int pos1 = -1;
    int pos2 = -1;
    read_count = 0;

    for (int i = 0; i < num_p; i++) {
      if (v[i]-> read != 1 && pt1 == NULL) { // o valor do vetor 1 só será atribuido se ele for nulo/vazio
        pt1 = v[i];
        pos1 = i;
        continue;
      }
      if (v[i]-> read != 1 && pt1 != NULL) { // o valor do vetor 2 só será atribuido se tiver um valor no vetor 1
        if (v[i] -> particao -> nome != pt1 -> particao -> nome){
             pt2 = v[i];
            pos2 = i;
            break;
        }
      }
    }

    if (pt1 == NULL){ // se o vetor 1 estiver vazio, aciona a flag 1
        flag1 = 1;
        printf("Acionada flag 1\n");
    }
    if (pt1 != NULL && pt2 == NULL){ // se o vetor 2 estiver vazio, aciona a flag 2
        flag2 = 1;
        printf("Acionada flag 1\n");
    }

    // nesse caso, todas as particoes foram lidas. entretanto, ainda há particoes para intercalar (mesmo que lidas)
    // então, iremos verificar se o numero da particao é menor que 0 ao invés de só verificar se ela foi lida

    if (flag1 == 1){
        for (int i = 0; i < num_p; i++) {
            if (v[i] -> particao -> num > 0 && pt1 == NULL){
                pt1 = v[i];
                pos1 = i;
                continue;
            }
            if (v[i] -> particao -> num > 0 && pt1 != NULL){
                if (v[i] -> particao -> nome != pt1 -> particao -> nome){
                        pt2 = v[i];
                        pos2 = i;
                        break;
                }
            }
        }
        printf("Tentativa de resolver flag 1\n");
    }

    if (flag2 == 1){
        for (int i = 0; i < num_p; i++) {
            if (v[i] -> particao -> num > 0 && pt1 != NULL){
                if (v[i] -> particao -> nome != pt1 -> particao -> nome){
                        pt2 = v[i];
                        pos2 = i;
                        break;
                }
            }
        }
         printf("Tentativa de resolver flag 2\n");
    }

    printf("\nCheckpoint 5: definiu os vetores para serem intercalados\n");
    if (pos1 > 0 && pos2 > 0){
        printf("1 nome: %s\n", v[pos1] -> particao -> nome);
        printf("2 nome: %s\n", v[pos2] -> particao -> nome);
    }


    // irá intercalar os vetores encontrado se eles não estiverem vazios
    // as flags são resetadas para verificar a proxima iteração, caso tiver

    if (pt1 != NULL && pt2 != NULL && pos2 != -1) {
      printf("\nCheckpoint ??: ha vetores para intercalar\n");
      // intercala_particoes_funcionarios(v[pos1], v[pos2], nome_particoes, (num_p + index));
      intercala_particoes_funcionarios(v[pos1], v[pos2], nome_particoes, (num_p + index));
      last = num_p + index; // salva o indice da ultima particao criada
      index++; // incrementa o indice caso precise criar mais uma particao posteriormente
    } else  {

        printf("\nCheckpoint 6: verifica se vetores foram intercalados\n");
        // irá verificar se todos os vetores foram intercalados
        for (int i = 0; i < num_p; i++) {
            if (v[i]->read == 1) {
                if (v[i] -> particao -> num < 0){
                    read_count++;
                }
            }
        }
        // se todos forem intercalados, irá finalizar a iteração
        if (read_count == num_p || read_count == num_p - 1) {
            for (int i = 0; i < num_p; i++){
                if (v[i] -> particao -> num != -1){
                    p_nome = v[i] -> particao -> nome;
                    count_sentinel ++;
                }
            }
            free(pt1);
            free(pt2);
            fim = 1;
        }

    }
        flag1 = 0;
        flag2 = 0;
        free(pt1);
        free(pt2);
    }

    printf("\nUltima particao: %s, Count: %d\n", p_nome, count_sentinel);

   printf("\nCheckpoint 7: terminou a intercalacao dos vetores\n");

  // nessa parte, será salvo o nome da ultima partição para o nome do arquivo final escolhido

  char *particao_final = malloc(40 * sizeof(char));
  sprintf(particao_final, "particoesFunc/particao%d", last);
  char *fim_ext = ".dat";
  strcat(particao_final, fim_ext);

    remove(out);
    rename(particao_final, out);
    printf("Remove checkpoint: renomeou arquivo\n");

    printf("Fim do processo de mesclagem.\n");
}


void intercalacao_funcionarios(char *nome_arquivo_saida, TNomes *nome_particoes) {
    int fim = 0;
    int num_p = nome_particoes->tam;
    FILE *out;
    int i = 0;

    printf("Checkpoint intercalacao 1\n");

    if ((out = fopen(nome_arquivo_saida, "a+")) == NULL) {
        printf("Erro ao abrir arquivo de saida");
        return;
    }

    TVetFunc **v = (TVetFunc **)malloc(num_p * sizeof(TVetFunc*));
    if (v == NULL) {
        printf("Erro ao alocar vetor de partições\n");
        return;
    }

    TParticao *aux_p = nome_particoes->primeiro->prox;
    printf("Checkpoint intercalacao 2: inicializando vetor\n\n");

    while (aux_p != NULL && i < num_p) {
        v[i] = (TVetFunc *)malloc(sizeof(TVetFunc));
        if (v[i] == NULL) {
            printf("Erro ao alocar TVetFunc na posição %d\n", i);
            exit(1);
        }

        char caminho_completo[100];
        sprintf(caminho_completo, "particoesFunc/%s", aux_p->nome);
        v[i]->f = fopen(caminho_completo, "r");

        printf("\nAbriu o arquivo: %s\n", aux_p->nome);

        v[i]->particao = aux_p;
        printf("Associou particao\n");

        if (v[i]->f != NULL) {
            fseek(v[i]->f, 0, SEEK_SET);
            TFunc *c = le_funcionario(v[i]->f);

            if (c == NULL) {
                printf("Funcionario NULL na partição %s\n", aux_p->nome);
                v[i]->func = funcionario(INT_MAX, "NULL", "-", "-", 0.0);
            } else {
                printf("Funcionario %d lido da partição %s\n", c->cod, aux_p->nome);
                v[i]->func = c;
            }

            v[i]->aux_p = 0;

        } else {
            printf("Não foi possível abrir o arquivo %s\n", aux_p->nome);
            fim = 1;
        }

        i++;
        aux_p = aux_p->prox;
    }

    printf("Checkpoint intercalacao 3: vetor preenchido \n");

    // aqui, o procedimento irá retornar um TVet v com num_p primeiros clientes
    // e seus arquivos nessa parte, vai ser ordenado o vetor TVet v

    int n = num_p;
    mergeSort_funcionarios(v, 0, n - 1);

    printf("Checkpoint intercalacao 4: mergesort\n");

    // intercala as entidades
    processa_particoes_funcionarios(v, nome_particoes, num_p, nome_arquivo_saida);

    // no final da função de processamento, é importante que tenha como abrir o arquivo "nome_arquivo_saida" novamente, para indicar que ocorreu tudo certo
    /*
        if ((out = fopen(nome_arquivo_saida, "r+")) == NULL) {
        printf("Erro ao abrir arquivo de saida");
        return;

    }
        int tamanho_arq = tamanho_arquivo_funcionario(out);
        insertion_sort_disco_funcionario(out, tamanho_arq);
    */

    printf("Checkpoint intercalacao 5: fim da escrita no arquivo final\n");

    free(aux_p);
    printf("Intercalacao completa.\n");

}

void intercala_particoes_funcionarios(TVetFunc *particao,TVetFunc *particao2, TNomes *nome_particoes, int index) {
  int p = 0, i = 0, signal = 0;
  int fim = 0;
  int numRegistros = NUM_REGISTROS;
  int num_p = nome_particoes->tam;

   printf("\nCheckpoint 2: entrou na funcao intercalar\n");
   print_particao(*particao -> particao);
   print_particao(*particao2 -> particao);

    // é criado o novo arquivo de partição para salvar o resultado da intercalação dos dois vetores
  char *nome = malloc(40 * sizeof(char));
  sprintf(nome, "particoesFunc/particao%d", index);
  char *fim_ext = ".dat";
  strcat(nome, fim_ext);
  FILE *out = fopen(nome, "wb+");

    printf("\nCheckpoint 2.2: criou o arquivo de particao.\n");

  int aux = 0;
  int tam_reg = tamanho_registro_funcionario();

    // são feitos dois vetores do tipo escolhido da entidade para comparar para escrita no arquivo final
  TFunc *p1[10];
  TFunc *p2[10];

  if(particao-> f == NULL){
    particao -> f = fopen(particao -> particao -> nome, "r+");
  }

  if(particao2 -> f == NULL){
    particao2 -> f = fopen(particao2 -> particao -> nome, "r+");
  }

  if (particao2 -> f == NULL || particao -> f == NULL){
    printf("ERROR: Nao foi possivel abrir arquivo de particao.\n");
    return;
  }

  fseek(particao-> f, 0 * tam_reg, SEEK_SET);
  fseek(particao2-> f, 0 * tam_reg, SEEK_SET);

    printf("\nCheckpoint 3: definindo os vetores da entidade..\n");
  for (i = 0; i < 10; i++) {
    p1[i] = le_funcionario(particao->f);
    if (p1[i] == NULL) {
          p1[i] = funcionario(INT_MAX, "Funcionario NULL", " ", " ", 0); // se não encontrado, define com INT_MAX o codigo

    }

    fseek(particao->f, i * tam_reg, SEEK_SET);
    particao->aux_p++;
  }

  for (i = 0; i < 10; i++) {
    p2[i] = le_funcionario(particao2->f);
    if (p2[i] == NULL) {
          p2[i] = funcionario(INT_MAX, "Funcionario NULL", " ", " ", 0); // se não encontrado, define com INT_MAX o codigo

    }
    fseek(particao2->f, i * tam_reg, SEEK_SET);
    particao2->aux_p++;
  }

printf("\nCheckpoint 3: definiu os vetores da entidade.\n");

  fseek(particao->f, 0 * tam_reg, SEEK_SET);
  fseek(particao2->f, 0 * tam_reg, SEEK_SET);
  fseek(out, 0 * tam_reg, SEEK_SET);

  int n_aux = 0; // variavel para armazenar o numero da particao correspondente

  while (!fim) { // escreve no arquivo de saida
    int menor = INT_MAX;
    int pos_menor = -1;


    for (int j = 0; j < 10; j++) { // irá verificar se algum codigo do primeiro vetor é menor que o salvo
      if (p1[j]->cod < menor) {
        menor = p1[j]->cod;
        n_aux = 1;
      }
    }

    for (int j = 0; j < 10; j++) { // depois, irá verificar se algum codigo do segundo vetor é menor que o menor do primeiro
      if (p2[j]->cod < menor) {
        menor = p2[j]->cod;
        n_aux = 2;
      }
    }

    if (pos_menor != -1) { // verifica se alguma entidade foi encontrada

      fseek(out, aux * tam_reg, SEEK_SET); // ajusta a posição para escrita no arquivo de saída
      if (n_aux == 1) {
        fseek(out, aux * tam_reg, SEEK_SET);
        salva_funcionario(p1[pos_menor], out);
        particao->aux_p++; // incrementa ponteiro para referencia da posição no arquivo da particao

        fseek(particao->f, particao->aux_p * tam_reg, SEEK_SET); // salva o proximo cliente do arquivo do ultimo salvo no vetor
        particao->func = le_funcionario(particao->f);

        if (particao->func == NULL) {
          particao->func = funcionario(INT_MAX, "Funcionario NULL", " ", " ", 0); // se não encontrado, define com INT_MAX o codigo
        }

        p1[pos_menor] = particao->func; //salva a entidade encontrada no vetor
        aux++; // incrementa ponteiro para referencia da posição no arquivo de saida

      } else if (n_aux == 2) {
        fseek(out, aux * tam_reg, SEEK_SET);
        salva_funcionario(p2[pos_menor], out);
        particao2->aux_p++;

        fseek(particao2->f, particao2->aux_p * tam_reg, SEEK_SET);
        particao2->func = le_funcionario(particao2->f);

        if (particao2->func == NULL) {
          particao2->func = funcionario(INT_MAX, "Funcionario NULL", " ", " ", 0);
        }

        p2[pos_menor] = particao2->func;
        aux++;
      }

    } else {
      fim = 1; // se o menor codigo por INT_MAX, significa que acabou a leitura das particoes
    }
  }

  fclose(particao->f);
  fclose(particao2->f);

// define o novo arquivo dos vetores como a ultima particao criada
  particao->f = out;
  particao2->f = out;

// reinicia o ponteiro para leitura
  particao->aux_p = 0;
  particao2->aux_p = 0;

// redefine o nome e indice de referencia da particao nos vetores

  particao->particao->nome = nome;
  particao->particao->num = index;

  particao2->particao->nome = nome;
  particao2->particao->num = -1;


// define como lido um dos vetores
  particao2->read = 1;

   printf("\nCheckpoint 3: saiu da funcao intercalar\n\n");
}

