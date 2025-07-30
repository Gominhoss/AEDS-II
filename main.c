#include "entidades.h"
#include "cliente.c"
#include "estoque.c"
#include "fornecedor.c"
#include "funcionario.c"
#include "produto.c"
#include "particoes.c"
#include "utils.c"
#include <stdio.h>
#include <stdlib.h>

void msg_MENU_inicial() {
  printf(
      "\n----------- PROGRAMA DE GERENCIAMENTO DE SUPERMERCADO -----------\n");
  printf("OBS.: Todas as informacoes serao armazenadas em arquivos.\nBASES "
         "DISPONIVEIS:\n");
  printf("1 - Funcionarios\n2 - Clientes\n3 - Fornecedores\n4 - Produtos\n5 - Estoque\n6 - Sair\n");
  printf("RESPOSTA: ");
}

void msg_MENU(char *tipo) {
  printf("\n----------- PROGRAMA DE GERENCIAMENTO DE %s -----------\n", tipo);
  printf("OBS.: Todas as informacoes serao armazenadas em "
         "arquivos.\n\nOPERACOES DISPONIVEIS:\n");
  printf("1 - Criar base\n2 - Ordenar\n3 - Imprimir\n4 - Pesquisar "
         "(Sequencial)\n5 - Pesquisar (Binaria)\n6 - Criar Particoes\n7 - "
         "Intercalacao\n8 - Sair\n");
  printf("RESPOSTA: ");
}


void msg_MENUP(char *tipo) {
  printf("\n----------- PROGRAMA DE GERENCIAMENTO DE %s -----------\n", tipo);
  printf("OBS.: Todas as informacoes serao armazenadas em "
         "arquivos.\n\nOPERACOES DISPONIVEIS:\n");
  printf("1 - Criar base\n2 - Ordenar\n3 - Imprimir\n4 - Pesquisar "
         "(Sequencial)\n5 - Pesquisar (Binaria)\n6 - Listar produtos de um fornecedor\n7 - Sair\n");
  printf("RESPOSTA: ");
}

void msg_MENUE(char *tipo) {
  printf("\n----------- PROGRAMA DE GERENCIAMENTO DE %s -----------\n", tipo);
  printf("OBS.: Todas as informacoes serao armazenadas em "
         "arquivos.\n\nOPERACOES DISPONIVEIS:\n");
  printf("1 - Criar base\n2 - Ordenar\n3 - Imprimir\n4 - Pesquisar "
         "(Sequencial)\n5 - Pesquisar (Binaria)\n6 - Estoques abaixo do mInimo\n7 - Sair\n");
  printf("RESPOSTA: ");
}

void menu_funcionario() {
    FILE *out;
    FILE *ordenado = NULL;
    int flag = 0;
    TNomes *conjunto_particoes;
    int qtdParticoes = 0;

    if ((out = fopen(FUNCIONARIOS_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    } else {
        int escolha = -1;
        int cod;

        apaga_particoes_existentes("particoesFunc");

        while (escolha != 8) {
            msg_MENU("FUNCIONARIO");
            scanf("%d", &escolha);
            if (escolha == 1) {
                printf("\nInforme quantos registros tera a base: ");
                int num;
                scanf("%d", &num);
                gerarBaseDesordenada_funcionario(out, num);
            } else if (escolha == 2) {
                insertion_sort_disco_funcionario(out, tamanho_arquivo_funcionario(out));
                printf("\n-----------------------------Base ordenada-----------------------");
                le_funcionarios(out);
            } else if (escolha == 3) {
                if (flag) {
                    if ((ordenado = fopen("func_intercalado.dat", "r+")) == NULL) {
                        printf("Erro ao abrir arquivo\n");
                        exit(1);
                    }
                    le_funcionarios(ordenado);
                    printf("\nArquivo lido: Ordenado por particoes\n");
                } else {
                    le_funcionarios(out);
                    printf("\nArquivo lido: Ordenado em disco\n");
                }
            } else if (escolha == 4) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                printf("\n-----------------------------Busca sequencial-----------------------");
                TFunc *func1 = busca_sequencial_funcionario(cod, out);
                printf("\n");
                if (func1 == NULL || func1->cod == 0) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_funcionario(func1);
                }
            } else if (escolha == 5) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                printf("\n\n-----------------------------Busca binaria-----------------------");
                TFunc *func = busca_binaria_funcionario(cod, out, tamanho_arquivo_funcionario(out));
                printf("\n");
                if (func == NULL || func->cod == 0) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_funcionario(func);
                }
            } else if (escolha == 6) {
                printf("Criando partições...");
                qtdParticoes = classificacaoSubs_func(out);  // agora retorna int
                conjunto_particoes = le_nomes_particoes_diretorio("particoesFunc");
            } else if (escolha == 7) {
                printf("Intercalando particoes...");
                if (lista_vazia(*conjunto_particoes)) {
                    printf("Nao existe particoes para intercalar.\n");
                    break;
                } else {
                    mostrar_particoes(*conjunto_particoes);
                    intercalacao_funcionarios_vencedores("func_intercalado.dat", qtdParticoes);
                    flag = 1;
                }
            } else if (escolha == 8) {
                system("cls");
                break;
            } else {
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
            }
        }
        fclose(out);
        if (ordenado) fclose(ordenado);
    }
}


void menu_cliente() {
    FILE *out_cli;
    FILE *ordenado = NULL;
    int flag = 0;
    TNomes *conjunto_particoes;
    int qtdParticoes = 0;

    if ((out_cli = fopen(CLIENTES_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    } else {
        int escolha = -1;
        int cod;

        apaga_particoes_existentes("particoesCli");

        while (escolha != 8) {
            msg_MENU("CLIENTE");
            scanf("%d", &escolha);
            if (escolha == 1) {
                printf("\nInforme quantos registros tera a base: ");
                int num;
                scanf("%d", &num);
                gerarBaseDesordenada_cliente(out_cli, num);
            } else if (escolha == 2) {
                insertion_sort_disco_cliente(out_cli, tamanho_arquivo_cliente(out_cli));
                printf("\n-----------------------------Base ordenada-----------------------");
                le_clientes(out_cli);
            } else if (escolha == 3) {
                if (flag) {
                    if ((ordenado = fopen("clientes_intercalado.dat", "r+")) == NULL) {
                        printf("Erro ao abrir arquivo\n");
                        exit(1);
                    }
                    le_clientes(ordenado);
                    printf("\nArquivo lido: Ordenado por particoes\n");
                } else {
                    le_clientes(out_cli);
                    printf("\nArquivo lido: Ordenado em disco\n");
                }
            } else if (escolha == 4) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                printf("\n-----------------------------Busca sequencial-----------------------");
                TCliente *c1 = busca_sequencial_cliente(cod, out_cli);
                printf("\n");
                if (c1 == NULL || c1->cod == 0) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_cliente(c1);
                }
            } else if (escolha == 5) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                printf("\n\n-----------------------------Busca binaria-----------------------");
                TCliente *c2 = busca_binaria_cliente(cod, out_cli, tamanho_arquivo_cliente(out_cli));
                printf("\n");
                if (c2 == NULL || c2->cod == 0) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_cliente(c2);
                }
            } else if (escolha == 6) {
                printf("Criando partições...");
                qtdParticoes = classificacaoSubs_cli(out_cli);
                conjunto_particoes = le_nomes_particoes_diretorio("particoesCli");
            } else if (escolha == 7) {
                printf("Intercalando particoes...");
                if (lista_vazia(*conjunto_particoes)) {
                    printf("Nao existe particoes para intercalar.\n");
                    break;
                } else {
                    mostrar_particoes(*conjunto_particoes);
                    intercalacao_clientes_vencedores("clientes_intercalado.dat", qtdParticoes);
                    flag = 1;
                }
            } else if (escolha == 8) {
                system("cls");
                break;
            } else {
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
            }
        }
        fclose(out_cli);
        if (ordenado) fclose(ordenado);
    }
}


void menu_fornecedor() {
    FILE *out;
    FILE *ordenado = NULL;
    TNomes *conjunto_particoes = NULL;
    int flag = 0;
    int qtdParticoes = 0;  // variável para armazenar o número de partições criadas

    if ((out = fopen(FORNECEDOR_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    } else {
        int escolha = -1;
        int cod;

        while (escolha != 8) {
            msg_MENU("FORNECEDORES");
            scanf("%d", &escolha);

            if (escolha == 1) {
                printf("\nInforme quantos registros tera a base: ");
                int num;
                scanf("%d", &num);
                gerarBaseDesordenada_fornecedor(out, num);

            } else if (escolha == 2) {
                insertion_sort_disco_fornecedor(out, tamanho_arquivo_fornecedor(out));
                printf("\n-----------------------------Base ordenada-----------------------\n");
                le_fornecedores(out);

            } else if (escolha == 3) {
                if (flag) {
                    if ((ordenado = fopen("forn_intercalado.dat", "r+")) == NULL) {
                        printf("Erro ao abrir arquivo\n");
                        exit(1);
                    }
                    le_fornecedores(ordenado);
                    printf("\nArquivo lido: Ordenado por particoes\n");
                } else {
                    if (out == NULL) {
                        printf("Erro ao abrir arquivo\n");
                        exit(1);
                    }
                    le_fornecedores(out);
                    printf("\nArquivo lido: Ordenado em disco\n");
                }

            } else if (escolha == 4) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);

                printf("\n-----------------------------Busca sequencial-----------------------\n");
                TForn forn = busca_sequencial_fornecedor(cod, out);
                printf("\n");
                if (forn.cod != cod) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_fornecedor(&forn);
                }

            } else if (escolha == 5) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);

                printf("\n\n-----------------------------Busca binaria-----------------------\n");
                TForn forn = busca_binaria_fornecedor(cod, out, tamanho_arquivo_fornecedor(out));
                printf("\n");
                if (forn.cod != cod) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_fornecedor(&forn);
                }

            } else if (escolha == 6) {
                printf("Criando partições...\n");
                qtdParticoes = classificacaoSubs_forn(out);  // recebe o número de partições criadas
                conjunto_particoes = le_nomes_particoes_diretorio("particoesForn");

            } else if (escolha == 7) {
                printf("Intercalando particoes...\n");

                if (conjunto_particoes == NULL || lista_vazia(*conjunto_particoes)) {
                    printf("Nao existe particoes para intercalar.\n");
                    break;
                } else {
                    mostrar_particoes(*conjunto_particoes);
                    intercalacao_fornecedores_vencedores("forn_intercalado.dat", qtdParticoes);  // passa qtdParticoes
                    flag = 1;
                }

            } else if (escolha == 8) {
                system("cls");
                break;

            } else {
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
            }
        }
        fclose(out);
        if (ordenado != NULL) fclose(ordenado);
    }
}

void menu_produto() {
    FILE *out_cli;
    FILE *ordenado = NULL;
    TNomes *conjunto_particoes;
    int flag = 0;
    int qtdParticoes = 0;

    if ((out_cli = fopen(PRODUTOS_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    } else {
        int escolha = -1;
        int cod;

        apaga_particoes_existentes("particoesProd");

        while (escolha != 8) {
            msg_MENU("PRODUTO");
            scanf("%d", &escolha);
            if (escolha == 1) {
                printf("\nInforme quantos registros tera a base: ");
                int num;
                scanf("%d", &num);
                gerarBaseDesordenada_produto(out_cli, num);
            } else if (escolha == 2) {
                insertion_sort_disco_produto(out_cli, tamanho_arquivo_produto(out_cli));
                printf("\n-----------------------------Base ordenada-----------------------");
                le_produtos(out_cli);
            } else if (escolha == 3) {
                if (flag) {
                    if ((ordenado = fopen("prod_intercalado.dat", "r+")) == NULL) {
                        printf("Erro ao abrir arquivo\n");
                        exit(1);
                    }
                    le_produtos(ordenado);
                    printf("\nArquivo lido: Ordenado por particoes\n");
                } else {
                    le_produtos(out_cli);
                    printf("\nArquivo lido: Ordenado em disco\n");
                }
            } else if (escolha == 4) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                printf("\n-----------------------------Busca sequencial-----------------------");
                TProd prod = busca_sequencial_produto(cod, out_cli);
                printf("\n");
                if (prod.cod != cod) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_produto(&prod);
                }
            } else if (escolha == 5) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                printf("\n\n-----------------------------Busca binaria-----------------------");
                TProd prod = busca_binaria_produto(cod, out_cli, tamanho_arquivo_produto(out_cli));
                printf("\n");
                if (prod.cod != cod) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_produto(&prod);
                }
            } else if (escolha == 6) {
                printf("Criando partições...");
                qtdParticoes = classificacaoSubs_prod(out_cli);
                conjunto_particoes = le_nomes_particoes_diretorio("particoesProd");
            } else if (escolha == 7) {
                printf("Intercalando particoes...");
                if (lista_vazia(*conjunto_particoes)) {
                    printf("Nao existe particoes para intercalar.\n");
                    break;
                } else {
                    mostrar_particoes(*conjunto_particoes);
                    intercalacao_produtos_vencedores("prod_intercalado.dat", qtdParticoes);
                    flag = 1;
                }
            } else if (escolha == 8) {
                system("cls");
                break;
            } else {
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
            }
        }
        fclose(out_cli);
        if (ordenado) fclose(ordenado);
    }
}


void menu_estoque() {
    FILE *out_cli;
    FILE *ordenado = NULL;
    TNomes *conjunto_particoes;
    int flag = 0;
    int qtdParticoes = 0;

    if ((out_cli = fopen(ESTOQUE_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo de estoque\n");
        exit(1);
    } else {
        int escolha = -1;
        int cod;

        apaga_particoes_existentes("particoesEstoq");

        while (escolha != 8) {
            msg_MENU("ESTOQUE");
            scanf("%d", &escolha);

            if (escolha == 1) {
                // gerando a base a partir do arquivo de produtos
                FILE *produtos = fopen(PRODUTOS_FILE, "rb");
                if (!produtos) {
                    printf("Erro ao abrir arquivo de produtos para gerar estoque\n");
                } else {
                    gerarBaseDesordenada_estoque(out_cli, produtos);
                    fclose(produtos);
                    printf("Base de estoque gerada a partir dos produtos.\n");
                }
            } else if (escolha == 2) {
                insertion_sort_disco_estoque(out_cli, tamanho_arquivo_estoque(out_cli));
                printf("\n-----------------------------Base ordenada-----------------------");
                le_estoques(out_cli);
            } else if (escolha == 3) {
                if (flag) {
                    if ((ordenado = fopen("estq_intercalado.dat", "r+")) == NULL) {
                        printf("Erro ao abrir arquivo\n");
                        exit(1);
                    }
                    le_estoques(ordenado);
                    printf("\nArquivo lido: Ordenado por particoes\n");
                } else {
                    le_estoques(out_cli);
                    printf("\nArquivo lido: Ordenado em disco\n");
                }
            } else if (escolha == 4) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                printf("\n-----------------------------Busca sequencial-----------------------\n");
                TEstoque est = busca_sequencial_estoque(cod, out_cli);
                printf("\n");
                if (est.cod != cod) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_estoque(&est);
                }
            } else if (escolha == 5) {
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                printf("\n-----------------------------Busca binaria-----------------------\n");
                TEstoque est = busca_binaria_estoque(cod, out_cli, tamanho_arquivo_estoque(out_cli));
                printf("\n");
                if (est.cod != cod) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_estoque(&est);
                }
            } else if (escolha == 6) {
                printf("Criando partições...");
                qtdParticoes = classificacaoSubs_estoq(out_cli);
                conjunto_particoes = le_nomes_particoes_diretorio("particoesEstoq");
            } else if (escolha == 7) {
                printf("Intercalando particoes...");
                if (lista_vazia(*conjunto_particoes)) {
                    printf("Nao existe particoes para intercalar.\n");
                    break;
                } else {
                    mostrar_particoes(*conjunto_particoes);
                    intercalacao_estoque_vencedores("estq_intercalado.dat", qtdParticoes);
                    flag = 1;
                }
            } else if (escolha == 8) {
                system("cls");
                break;
            } else {
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
            }
        }
        fclose(out_cli);
        if (ordenado) fclose(ordenado);
    }
}

int main(int argc, char **argv) {
  int option = -1;

  while (option != 6) {
    msg_MENU_inicial();
    scanf("%d", &option);

    if (option == 1) {
      menu_funcionario();

    } else if (option == 2) {
      menu_cliente();

    } else if (option == 3) {
      menu_fornecedor();

    } else if (option == 4) {
      menu_produto();

    } else if (option == 5) {
      menu_estoque();

    } else if (option == 6) {
      printf("\nFIM DO PROGRAMA!\n");
      system("cls");
      break;
    } else {
      printf("\nESCOLHA UMA OPCAO VALIDA!\n");
      break;
    }
  }
}
