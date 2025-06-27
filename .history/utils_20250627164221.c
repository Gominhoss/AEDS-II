#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "utils.h"

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

void embaralhar_arquivo_fornecedor(FILE *arq, int total_records) {
    if (total_records <= 1) return;
    srand(time(NULL));

    for (int i = 0; i < total_records - 1; i++) {
        int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

        // Troca os registros nas posições i e j
        fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
        TForn *reg_i = le_fornecedor(arq);

        fseek(arq, j * tamanho_registro_fornecedor(), SEEK_SET);
        TForn *reg_j = le_fornecedor(arq);

        fseek(arq, i * tamanho_registro_fornecedor(), SEEK_SET);
        salva_fornecedor(reg_j, arq);

        fseek(arq, j * tamanho_registro_fornecedor(), SEEK_SET);
        salva_fornecedor(reg_i, arq);

        free(reg_i);
        free(reg_j);
    }
    fflush(arq);
}

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