// NOVO ARQUIVO: hash.h

#ifndef HASH_H
#define HASH_H

#include "funcionario.h" // Inclui a definição de TFunc

#define TAMANHO_TABELA_HASH 101

// Protótipos
int funcao_hash_funcionario(int cod);
void criar_tabela_hash_funcionario(FILE *tabela_hash);
void construir_hash_da_base_existente(FILE *tabela_hash, FILE *arq_funcionarios);
void inserir_novo_funcionario_hash(FILE *tabela_hash, FILE *arq_funcionarios);
TFunc* buscar_funcionario_hash(int cod, FILE *tabela_hash, FILE *arq_funcionarios);
void remover_funcionario_hash(int cod, FILE *tabela_hash, FILE *arq_funcionarios);
void imprimir_tabela_hash_completa(FILE *tabela_hash, FILE *arq_funcionarios);

#endif