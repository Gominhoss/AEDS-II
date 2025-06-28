#include "entidades.h"
#include "cliente.c"
#include "estoque.c"
#include "fornecedor.c"
#include "funcionario.c"
#include "produto.c"
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
         "(Sequencial)\n5 - Pesquisar (Binaria)\n6 - Sair\n");
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
  if ((out = fopen(FUNCIONARIOS_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

    while (escolha != 6) {
      msg_MENU("FUNCIONARIO");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        gerarBaseDesordenada_funcionario(out, num);
      } else if (escolha == 2) {
        selection_sort_disco_funcionario(out, tamanho_arquivo_funcionario(out));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_funcionarios(out);
      } else if (escolha == 3) {
        le_funcionarios(out);

      } else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TFunc *func1 = busca_sequencial_funcionario(cod, out);
        printf("\n");
        if (func1->cod == 0 || func1 == NULL) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_funcionario(func1);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TFunc *func = busca_binaria_funcionario(
            cod, out, tamanho_arquivo_funcionario(out));
        printf("\n");

        if (func->cod == 0 || func == NULL) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_funcionario(func);
        }
      } else if (escolha == 6) {
        system("cls");
        break;
      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out);
  }
}

void menu_cliente() {
  FILE *out_cli;
  if ((out_cli = fopen(CLIENTES_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

    while (escolha != 6) {
      msg_MENU("CLIENTE");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        gerarBaseDesordenada_cliente(out_cli, num);
      } else if (escolha == 2) {
        selection_sort_disco_cliente(out_cli, tamanho_arquivo_cliente(out_cli));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_clientes(out_cli);
      } else if (escolha == 3) {
        le_clientes(out_cli);

      } else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TCliente *c1 = busca_sequencial_cliente(cod, out_cli);
        printf("\n");
        if (c1->cod == 0 || c1 == NULL) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_cliente(c1);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TCliente *c2 = busca_binaria_cliente(cod, out_cli,
                                             tamanho_arquivo_cliente(out_cli));
        printf("\n");
        if (c2->cod == 0 || c2 == NULL) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_cliente(c2);
        }
      } else if (escolha == 6) {
        system("cls");
        break;
      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out_cli);
  }
}

void menu_fornecedor() {
  FILE *out;
  if ((out = fopen(FORNECEDOR_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

    while (escolha != 7) {
      msg_MENU("FORNECEDORES");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        gerarBaseDesordenada_fornecedor(out, num);
      } else if (escolha == 2) {
        selection_sort_disco_fornecedor(out, tamanho_arquivo_fornecedor(out));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_fornecedores(out);
      } else if (escolha == 3) {
        le_fornecedores(out);
      } else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
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

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TForn forn =
            busca_binaria_fornecedor(cod, out, tamanho_arquivo_fornecedor(out));
        printf("\n");
        if (forn.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_fornecedor(&forn);
        }
      } else if (escolha == 6) {
        system("cls");
        break;
      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out);
  }
}

void menu_produto() {
  FILE *out_cli;
  if ((out_cli = fopen(PRODUTOS_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

    while (escolha != 7) {
      msg_MENUP("PRODUTO");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        gerarBaseDesordenada_produto(out_cli, num);
      } else if (escolha == 2) {
        selection_sort_disco_produto(out_cli, tamanho_arquivo_produto(out_cli));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_produtos(out_cli);
      } else if (escolha == 3) {
        le_produtos(out_cli);

      } else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
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

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TProd prod = busca_binaria_produto(cod, out_cli,
                                           tamanho_arquivo_produto(out_cli));
        printf("\n");

        if (prod.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_produto(&prod);
        }
      } else if (escolha == 6) {
        printf("\n\n-----------------------------Pesquisar produto por fornecedor "
               "-----------------------");      
        int cod_forn;
        printf("\nInforme o codigo do fornecedor: ");
        scanf("%d", &cod_forn);

        FILE *arq_prod = fopen(PRODUTOS_FILE, "rb");
        if (!arq_prod) {
          printf("Erro ao abrir arquivo de produtos.\n");
          continue;
        }

        printf("\nProdutos fornecidos pelo fornecedor %d:\n", cod_forn);

        TProd *p;
        while ((p = le_produto(arq_prod)) != NULL) {
          if (p->cod_forn == cod_forn) {
            imprime_produto(p);
          }
          free(p);
        }
        fclose(arq_prod);
      } else if (escolha == 7) {
        system("cls");
        break;
      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out_cli);
  }
}

void menu_estoque() {
  FILE *out_cli;
  if ((out_cli = fopen(ESTOQUE_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo de estoque\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

    while (escolha != 7) {
      msg_MENUE("ESTOQUE");
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
        selection_sort_disco_estoque(out_cli, tamanho_arquivo_estoque(out_cli));
        printf("\n-----------------------------Base ordenada-----------------------\n");
        le_estoques(out_cli);

      } else if (escolha == 3) {
        le_estoques(out_cli);

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
        listar_estoques_abaixo_do_minimo(out_cli);

      } else if (escolha == 7) {
        system("cls");
        break;

      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
      }
    }
    fclose(out_cli);
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
