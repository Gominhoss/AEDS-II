/**
 * @file utils.h
 * @brief Protótipos de funções utilitárias gerais.
 *
 * Este arquivo contém protótipos para funções de propósito geral que podem ser
 * usadas em várias partes do sistema, como embaralhar um vetor.
 */

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdarg.h>
#include <time.h>

/**
 * @brief Embaralha os elementos de um vetor de inteiros.
 * Utiliza o algoritmo de Fisher-Yates.
 * @param vet Ponteiro para o vetor de inteiros a ser embaralhado.
 * @param vetSize O número de elementos no vetor.
 */
void embaralhar(int *vet, size_t vetSize);

#endif // UTILS_H_INCLUDED
