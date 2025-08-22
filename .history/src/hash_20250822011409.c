// NOVO ARQUIVO: hash.c

#include "hash.h"

// A função hash transforma a chave (código) num endereço (índice) na tabela.
int funcao_hash_funcionario(int cod) {
    return cod % TAMANHO_TABELA_HASH;
}

// Prepara o arquivo da tabela hash, preenchendo com -1 (vazio).
void criar_tabela_hash_funcionario(FILE *tabela_hash) {
    rewind(tabela_hash);
    ftruncate(fileno(tabela_hash), 0);

    int pos_vazia = -1;
    printf("Inicializando tabela hash com %d posicoes...\n", TAMANHO_TABELA_HASH);
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        fwrite(&pos_vazia, sizeof(int), 1, tabela_hash);
    }
    printf("Tabela Hash 'hash_funcionarios.dat' criada e inicializada.\n");
}

// Função interna para conectar um funcionário à lista encadeada na hash.
void inserir_na_lista_hash(FILE *tabela_hash, FILE *arq_funcionarios, int pos_novo_func, int cod_novo_func) {
    int hash_addr = funcao_hash_funcionario(cod_novo_func);
    fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
    
    int pos_cabeca_lista;
    fread(&pos_cabeca_lista, sizeof(int), 1, tabela_hash);

    if (pos_cabeca_lista == -1) {
        fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
        fwrite(&pos_novo_func, sizeof(int), 1, tabela_hash);
    } else {
        int pos_atual = pos_cabeca_lista;
        TFunc *func_percorrido;

        while (1) {
            fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
            func_percorrido = le_funcionario(arq_funcionarios);
            
            if (func_percorrido->prox == -1) {
                func_percorrido->prox = pos_novo_func;
                
                fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
                salva_funcionario(func_percorrido, arq_funcionarios);
                
                free(func_percorrido);
                break;
            }
            
            pos_atual = func_percorrido->prox;
            free(func_percorrido);
        }
    }
}

// Lê o arquivo de funcionários e constrói a tabela hash do zero.
void construir_hash_da_base_existente(FILE *tabela_hash, FILE *arq_funcionarios) {
    printf("\nConstruindo tabela hash a partir da base de dados...\n");
    criar_tabela_hash_funcionario(tabela_hash);
    rewind(arq_funcionarios);

    TFunc *func_lido;
    int pos_atual = 0;

    while ((func_lido = le_funcionario(arq_funcionarios)) != NULL) {
        long int pos_leitura_continua = ftell(arq_funcionarios);

        if (func_lido->ocupado == 1) {
            inserir_na_lista_hash(tabela_hash, arq_funcionarios, pos_atual, func_lido->cod);
        }

        fseek(arq_funcionarios, pos_leitura_continua, SEEK_SET);
        free(func_lido);
        pos_atual++;
    }
    printf("Construção da tabela hash concluída.\n");
}

// Busca um funcionário usando a tabela hash.
TFunc* buscar_funcionario_hash(int cod, FILE *tabela_hash, FILE *arq_funcionarios) {
    int hash_addr = funcao_hash_funcionario(cod);
    printf("Buscando codigo %d no endereco de hash %d...\n", cod, hash_addr);

    fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
    int pos_atual;
    if (fread(&pos_atual, sizeof(int), 1, tabela_hash) == 0 || pos_atual == -1) {
        printf("Nenhum funcionario encontrado nesse endereco.\n");
        return NULL;
    }
    
    TFunc* func_atual;
    while (pos_atual != -1) {
        fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
        func_atual = le_funcionario(arq_funcionarios);

        if (func_atual != NULL && func_atual->cod == cod && func_atual->ocupado == 1) {
            printf("Funcionario encontrado na posicao fisica %d do arquivo de dados.\n", pos_atual);
            return func_atual; // SUCESSO! Retorna o ponteiro. O chamador deve usar free().
        }
        
        pos_atual = func_atual->prox;
        free(func_atual); // Libera a memória se não for o funcionário certo.
    }

    printf("Funcionario com codigo %d nao encontrado na lista de colisao.\n", cod);
    return NULL;
}

// Remove um funcionário (remoção lógica).
void remover_funcionario_hash(int cod, FILE *tabela_hash, FILE *arq_funcionarios) {
    int hash_addr = funcao_hash_funcionario(cod);

    fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
    int pos_atual;
    if (fread(&pos_atual, sizeof(int), 1, tabela_hash) == 0 || pos_atual == -1) {
        printf("ERRO: Funcionario nao encontrado.\n");
        return;
    }
    
    int pos_anterior = -1;
    TFunc *func_atual = NULL;

    while (pos_atual != -1) {
        fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
        func_atual = le_funcionario(arq_funcionarios);

        if (func_atual != NULL && func_atual->cod == cod && func_atual->ocupado == 1) {
            // ETAPA 1: REMOÇÃO LÓGICA
            func_atual->ocupado = 0;
            fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
            salva_funcionario(func_atual, arq_funcionarios);
            
            // ETAPA 2: RELIGAR PONTEIROS
            if (pos_anterior == -1) {
                int proxima_pos = func_atual->prox;
                fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
                fwrite(&proxima_pos, sizeof(int), 1, tabela_hash);
            } else {
                fseek(arq_funcionarios, pos_anterior * tamanho_registro_funcionario(), SEEK_SET);
                TFunc *func_anterior = le_funcionario(arq_funcionarios);
                func_anterior->prox = func_atual->prox;
                fseek(arq_funcionarios, pos_anterior * tamanho_registro_funcionario(), SEEK_SET);
                salva_funcionario(func_anterior, arq_funcionarios);
                free(func_anterior);
            }
            
            printf("Funcionario de codigo %d removido com sucesso.\n", cod);
            free(func_atual);
            return;
        }

        pos_anterior = pos_atual;
        pos_atual = func_atual->prox;
        free(func_atual);
    }

    printf("ERRO: Funcionario de codigo %d nao foi encontrado para remocao.\n", cod);
}