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



// Função principal do Merge Sort
void mergeSort_clientes(TVetClientes **arr, int l, int r) {
  // printf("\nEntrou no MergeSort.\n");
  if (l < r) {
    int m = l + (r - l) / 2;

    // Ordenando a primeira e a segunda metade do array
    mergeSort_clientes(arr, l, m);
    mergeSort_clientes(arr, m + 1, r);

    // Mesclando as duas metades ordenadas
    merge_clientes(arr, l, m, r);
  }
}

void classificacaoSubs_cli(FILE *arq) {
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

// Função para mesclar duas metades do array em ordem crescente
void merge_clientes(TVetClientes **arr, int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  // Criando arrays temporários para armazenar as duas metades
  TVetClientes **L = (TVetClientes **)malloc(n1 * sizeof(TVetClientes *));
  TVetClientes **R = (TVetClientes **)malloc(n2 * sizeof(TVetClientes *));

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
    if (L[i]->cli->cod <= R[j]->cli->cod) {

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

void processa_particoes_clientes(TVetClientes **v, TNomes *nome_particoes,int num_p, char *out) {
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
    intercala_particoes_clientes(v[i], v[j], nome_particoes, (num_p + index));
    index++;
    j += 2;
  }

   printf("\nCheckpoint 4: saiu da intercalacao inicial do processar\n");

    int flag1 = 0;
    int flag2 = 0;
    char *p_nome;
    int count_sentinel = 0;

  while (!fim) { // nessa segunda iteração, irá verificar e intercalar os vetores que não tiverem sinalizados de "read" (lidos)
    TVetClientes *pt1 = NULL;
    TVetClientes *pt2 = NULL;
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
      intercala_particoes_clientes(v[pos1], v[pos2], nome_particoes, (num_p + index));
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
  sprintf(particao_final, "particoesCli/particao%d", last);
  char *fim_ext = ".dat";
  strcat(particao_final, fim_ext);

    remove(out);
    rename(particao_final, out);
    printf("Remove checkpoint: renomeou arquivo\n");

    printf("Fim do processo de mesclagem.\n");
}

void intercalacao_clientes(char *nome_arquivo_saida, TNomes *nome_particoes) {
    int fim = 0;
    int num_p = nome_particoes->tam;
    FILE *out;
    int i = 0;

  printf("Checkpoint intercalacao 1\n");

  if ((out = fopen(nome_arquivo_saida, "a+")) == NULL) { // arquivo final aberto em modo "append"
    printf("Erro ao abrir arquivo de saida");
    return;

  }

    TVetClientes **v = (TVetClientes **)malloc(num_p * sizeof(TVetClientes)); // matriz de ponteiros para os vetores da entidade
    TParticao *aux_p = nome_particoes->primeiro->prox; // ponteiro para particoes
    printf("Checkpoint intercalacao 2: inicializando vetor\n\n");

    while (aux_p != NULL) { // verifica a primeira entidade em todas as particoes

      v[i] = (TVetClientes *)malloc(sizeof(TVetClientes));
      v[i]->f = fopen(aux_p->nome, "r");
      printf("\nAbriu o arquivo: %s\n", aux_p->nome);

      v[i]->particao = aux_p;
      printf("Associou particao\n");

      if (v[i]->f != NULL) {
        fseek(v[i]->f, 0, SEEK_SET);
        TCliente *c = le_cliente(v[i]->f);

        printf("Leu entidade\n");
        if (c == NULL) { // indicativo do fim de clientes para leitura
          v[i]-> cli = cliente(INT_MAX, "Cliente NULL", "XXX.XXX.XXX-XX",
                            "XX/XX/XXXX", "xxxx@email.com");
        } else {
          v[i]-> cli = c;
        }
        v[i]->aux_p = 0;

      } else { // indicativo do fim das particoes para leitura
        printf("Nao foi possivel abrir o arquivo.\n\n");
        fim = 1;
      }
      i++;
      // printf("\n%s\n", aux_p -> nome);
      aux_p = aux_p->prox; // vai para o proximo arquivo
    }

    printf("Checkpoint intercalacao 3: vetor preenchido\n");

    // aqui, o procedimento irá retornar um TVet v com num_p primeiros clientes
    // e seus arquivos nessa parte, vai ser ordenado o vetor TVet v

    int n = num_p;
    mergeSort_clientes(v, 0, n - 1);

    printf("Checkpoint intercalacao 4: mergesort\n");

    // intercala as entidades
    processa_particoes_clientes(v, nome_particoes, num_p, nome_arquivo_saida);

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

void intercala_particoes_clientes(TVetClientes *particao, TVetClientes *particao2, TNomes *nome_particoes, int index) {
  int p = 0, i = 0, signal = 0;
  int fim = 0;
  int numRegistros = NUM_REGISTROS;
  int num_p = nome_particoes->tam;

   printf("\nCheckpoint 2: entrou na funcao intercalar\n");
   print_particao(*particao -> particao);
   print_particao(*particao2 -> particao);

    // é criado o novo arquivo de partição para salvar o resultado da intercalação dos dois vetores
  char *nome = malloc(40 * sizeof(char));
  sprintf(nome, "particoesCli/particao%d", index);
  char *fim_ext = ".dat";
  strcat(nome, fim_ext);
  FILE *out = fopen(nome, "wb+");

    printf("\nCheckpoint 2.2: criou o arquivo de particao.\n");

  int aux = 0;
  int tam_reg = tamanho_registro_cliente();

    // são feitos dois vetores do tipo escolhido da entidade para comparar para escrita no arquivo final
  TCliente *p1[10];
  TCliente *p2[10];

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
    p1[i] = le_cliente(particao->f);
    if (p1[i] == NULL) {
          p1[i] = cliente(INT_MAX, "Cliente NULL", "XXX.XXX.XXX-XX",
                            "XX/XX/XXXX", "xxxx@email.com");
    }

    fseek(particao->f, i * tam_reg, SEEK_SET);
    particao->aux_p++;
  }

  for (i = 0; i < 10; i++) {
    p2[i] = le_cliente(particao2->f);
    if (p2[i] == NULL) {
          p2[i] = cliente(INT_MAX, "Cliente NULL", "XXX.XXX.XXX-XX",
                            "XX/XX/XXXX", "xxxx@email.com");
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
        salva_cliente(p1[pos_menor], out);
        particao->aux_p++; // incrementa ponteiro para referencia da posição no arquivo da particao

        fseek(particao->f, particao->aux_p * tam_reg, SEEK_SET); // salva o proximo cliente do arquivo do ultimo salvo no vetor
        particao->cli = le_cliente(particao->f);

        if (particao->cli == NULL) {
          particao->cli = cliente(INT_MAX, "Cliente NULL", "XXX.XXX.XXX-XX",
                            "XX/XX/XXXX", "xxxx@email.com"); // se não encontrado, define com INT_MAX o codigo
        }

        p1[pos_menor] = particao->cli; //salva a entidade encontrada no vetor
        aux++; // incrementa ponteiro para referencia da posição no arquivo de saida

      } else if (n_aux == 2) {
        fseek(out, aux * tam_reg, SEEK_SET);
        salva_cliente(p2[pos_menor], out);
        particao2->aux_p++;

        fseek(particao2->f, particao2->aux_p * tam_reg, SEEK_SET);
        particao2->cli = le_cliente(particao2->f);

        if (particao2->cli == NULL) {
          particao2->cli= cliente(INT_MAX, "Cliente NULL", "XXX.XXX.XXX-XX",
                            "XX/XX/XXXX", "xxxx@email.com");
        }

        p2[pos_menor] = particao2->cli;
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