/**
 * @file main.c
 * @brief Ponto de entrada principal do programa de gerenciamento de supermercado.
 *
 * Este arquivo contém a função main e os menus para interagir com as diferentes
 * funcionalidades do sistema, como gerenciar funcionários, clientes, produtos, etc.
 */

#include "entidades.h"
#include <stdio.h>
#include <stdlib.h>

// Protótipos dos menus para evitar warnings
void menu_funcionario();
void menu_cliente();
void menu_fornecedor();
void menu_produto();
void menu_estoque();

/**
 * @brief Exibe o menu principal da aplicação.
 */
void msg_MENU_inicial() {
  printf(
      "\n----------- PROGRAMA DE GERENCIAMENTO DE SUPERMERCADO -----------\\n");
  printf("OBS.: Todas as informacoes serao armazenadas em arquivos.\nBASES "
             "DISPONIVEIS:\n");
  printf("1 - Funcionarios\n2 - Clientes\n3 - Fornecedores\n4 - Produtos\n5 - Estoque\n6 - Sair\n");
  printf("RESPOSTA: ");
}

/**
 * @brief Exibe o menu de operações padrão para uma entidade.
 * @param tipo String contendo o nome da entidade (ex: "FUNCIONARIO").
 */
void msg_MENU(char *tipo) {
  printf("\n----------- PROGRAMA DE GERENCIAMENTO DE %s -----------\\n", tipo);
  printf("OBS.: Todas as informacoes serao armazenadas em "
         "arquivos.\n\nOPERACOES DISPONIVEIS:\n");
  printf("1 - Criar base\n2 - Ordenar\n3 - Imprimir\n4 - Pesquisar "
         "(Sequencial)\n5 - Pesquisar (Binaria)\n6 - Sair\n");
  printf("RESPOSTA: ");
}

/**
 * @brief Menu de operações para a entidade Estoque.
 */
void msg_MENUE(char *tipo) {
  printf("\n----------- PROGRAMA DE GERENCIAMENTO DE %s -----------\\n", tipo);
  printf("OBS.: Todas as informacoes serao armazenadas em "
         "arquivos.\n\nOPERACOES DISPONIVEIS:\n");
  printf("1 - Criar base\n2 - Ordenar\n3 - Imprimir\n4 - Pesquisar "
         "(Sequencial)\n5 - Pesquisar (Binaria)\n6 - Estoques abaixo do minimo\n7 - Sair\n");
  printf("RESPOSTA: ");
}

/**
 * @brief Gerencia as operações relacionadas aos funcionários.
 */
void menu_funcionario() {
    FILE *out;
    if ((out = fopen(FUNCIONARIOS_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    
    int escolha = -1;
    int cod;

    while (escolha != 6) {
        msg_MENU("FUNCIONARIO");
        scanf("%d", &escolha);
        switch (escolha) {
            case 1:
                printf("\nInforme quantos registros tera a base: ");
                int num;
                scanf("%d", &num);
                gerarBaseDesordenada_funcionario(out, num);
                break;
            case 2:
                selection_sort_disco_funcionario(out, tamanho_arquivo_funcionario(out));
                printf("\n-----------------------------Base ordenada-----------------------\\n");
                le_funcionarios(out);
                break;
            case 3:
                le_funcionarios(out);
                break;
            case 4:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TFunc *func1 = busca_sequencial_funcionario(cod, out);
                if (func1 == NULL) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_funcionario(func1);
                    free(func1);
                }
                break;
            case 5:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TFunc *func = busca_binaria_funcionario(cod, out, tamanho_arquivo_funcionario(out));
                if (func == NULL) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_funcionario(func);
                    free(func);
                }
                break;
            case 6:
                system("cls");
                break;
            default:
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
        }
    }
    fclose(out);
}

/**
 * @brief Gerencia as operações relacionadas aos clientes.
 */
void menu_cliente() {
    FILE *out_cli;
    if ((out_cli = fopen(CLIENTES_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    int escolha = -1;
    int cod;

    while (escolha != 6) {
        msg_MENU("CLIENTE");
        scanf("%d", &escolha);
        switch (escolha) {
            case 1:
                printf("\nInforme quantos registros tera a base: ");
                int num;
                scanf("%d", &num);
                gerarBaseDesordenada_cliente(out_cli, num);
                break;
            case 2:
                selection_sort_disco_cliente(out_cli, tamanho_arquivo_cliente(out_cli));
                printf("\n-----------------------------Base ordenada-----------------------\\n");
                le_clientes(out_cli);
                break;
            case 3:
                le_clientes(out_cli);
                break;
            case 4:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TCliente *c1 = busca_sequencial_cliente(cod, out_cli);
                if (c1 == NULL) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_cliente(c1);
                    free(c1);
                }
                break;
            case 5:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TCliente *c2 = busca_binaria_cliente(cod, out_cli, tamanho_arquivo_cliente(out_cli));
                if (c2 == NULL) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_cliente(c2);
                    free(c2);
                }
                break;
            case 6:
                system("cls");
                break;
            default:
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
        }
    }
    fclose(out_cli);
}

/**
 * @brief Gerencia as operações relacionadas aos fornecedores.
 */
void menu_fornecedor() {
    FILE *out;
    if ((out = fopen(FORNECEDOR_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    int escolha = -1;
    int cod;

    while (escolha != 6) {
        msg_MENU("FORNECEDOR");
        scanf("%d", &escolha);
        switch (escolha) {
            case 1:
                printf("\nInforme quantos registros tera a base: ");
                int num;
                scanf("%d", &num);
                gerarBaseDesordenada_fornecedor(out, num);
                break;
            case 2:
                selection_sort_disco_fornecedor(out, tamanho_arquivo_fornecedor(out));
                printf("\n-----------------------------Base ordenada-----------------------\\n");
                le_fornecedores(out);
                break;
            case 3:
                le_fornecedores(out);
                break;
            case 4:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TForn forn_seq = busca_sequencial_fornecedor(cod, out);
                if (forn_seq.cod == -1) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_fornecedor(&forn_seq);
                }
                break;
            case 5:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TForn forn_bin = busca_binaria_fornecedor(cod, out, tamanho_arquivo_fornecedor(out));
                if (forn_bin.cod == -1) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_fornecedor(&forn_bin);
                }
                break;
            case 6:
                system("cls");
                break;
            default:
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
        }
    }
    fclose(out);
}

/**
 * @brief Gerencia as operações relacionadas aos produtos.
 */
void menu_produto() {
    FILE *out_prod;
    if ((out_prod = fopen(PRODUTOS_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    int escolha = -1;
    int cod;

    while (escolha != 6) {
        msg_MENU("PRODUTO");
        scanf("%d", &escolha);
        switch (escolha) {
            case 1:
                printf("\nInforme quantos registros tera a base: ");
                int num;
                scanf("%d", &num);
                gerarBaseDesordenada_produto(out_prod, num);
                break;
            case 2:
                selection_sort_disco_produto(out_prod, tamanho_arquivo_produto(out_prod));
                printf("\n-----------------------------Base ordenada-----------------------\\n");
                le_produtos(out_prod);
                break;
            case 3:
                le_produtos(out_prod);
                break;
            case 4:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TProd prod_seq = busca_sequencial_produto(cod, out_prod);
                if (prod_seq.cod == -1) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_produto(&prod_seq);
                }
                break;
            case 5:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TProd prod_bin = busca_binaria_produto(cod, out_prod, tamanho_arquivo_produto(out_prod));
                if (prod_bin.cod == -1) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_produto(&prod_bin);
                }
                break;
            case 6:
                system("cls");
                break;
            default:
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
        }
    }
    fclose(out_prod);
}

/**
 * @brief Gerencia as operações relacionadas ao estoque.
 */
void menu_estoque() {
    FILE *out_est;
    if ((out_est = fopen(ESTOQUE_FILE, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo de estoque\n");
        exit(1);
    }

    int escolha = -1;
    int cod;

    while (escolha != 7) {
        msg_MENUE("ESTOQUE");
        scanf("%d", &escolha);
        switch (escolha) {
            case 1:
                {
                    FILE *produtos = fopen(PRODUTOS_FILE, "rb");
                    if (!produtos) {
                        printf("Erro: Arquivo de produtos '%s' nao encontrado. Crie a base de produtos primeiro.\n", PRODUTOS_FILE);
                    } else {
                        gerarBaseDesordenada_estoque(out_est, produtos);
                        fclose(produtos);
                        printf("Base de estoque gerada a partir dos produtos.\n");
                    }
                }
                break;
            case 2:
                selection_sort_disco_estoque(out_est, tamanho_arquivo_estoque(out_est));
                printf("\n-----------------------------Base ordenada-----------------------\\n");
                le_estoques(out_est);
                break;
            case 3:
                le_estoques(out_est);
                break;
            case 4:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TEstoque est_seq = busca_sequencial_estoque(cod, out_est);
                if (est_seq.cod == -1) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_estoque(&est_seq);
                }
                break;
            case 5:
                printf("\nInforme o codigo a ser buscado: ");
                scanf("%d", &cod);
                TEstoque est_bin = busca_binaria_estoque(cod, out_est, tamanho_arquivo_estoque(out_est));
                if (est_bin.cod == -1) {
                    printf("Nao foi possivel encontrar o codigo solicitado.\n");
                } else {
                    imprime_estoque(&est_bin);
                }
                break;
            case 6:
                listar_estoques_abaixo_do_minimo(out_est);
                break;
            case 7:
                system("cls");
                break;
            default:
                printf("\nESCOLHA UMA OPCAO VALIDA!\n");
                break;
        }
    }
    fclose(out_est);
}

/**
 * @brief Função principal que executa o menu inicial e direciona para os submenus.
 * @return 0 em caso de sucesso.
 */
int main() {
  int option = -1;

  while (option != 6) {
    msg_MENU_inicial();
    scanf("%d", &option);

    switch (option) {
        case 1:
            menu_funcionario();
            break;
        case 2:
            menu_cliente();
            break;
        case 3:
            menu_fornecedor();
            break;
        case 4:
            menu_produto();
            break;
        case 5:
            menu_estoque();
            break;
        case 6:
            printf("\nFIM DO PROGRAMA!\n");
            system("cls");
            break;
        default:
            printf("\nESCOLHA UMA OPCAO VALIDA!\n");
            // Limpa o buffer de entrada para evitar loop infinito se o usuário digitar um caractere
            while(getchar() != '\n');
            break;
    }
  }
  return 0;
}
