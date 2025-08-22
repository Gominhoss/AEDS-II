/**
 * @file utils.c
 * @brief Implementação de funções utilitárias gerais.
 */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "utils.h"

/**
 * @brief Embaralha os elementos de um vetor de inteiros usando o algoritmo de Fisher-Yates.
 * @param vet Ponteiro para o vetor de inteiros a ser embaralhado.
 * @param vetSize O número de elementos no vetor.
 */
void embaralhar(int *vet, size_t vetSize) {
    // Inicializa o gerador de números aleatórios com uma semente baseada no tempo atual
    srand(time(NULL));

    if(vetSize > 1){
        size_t i;
        // Itera do primeiro ao penúltimo elemento
        for(i = 0; i < vetSize - 1; i++){
          // Escolhe um índice j aleatório tal que i <= j < vetSize
          size_t j = i + rand() / (RAND_MAX / (vetSize - i) + 1);
          
          // Troca os elementos nas posições i e j
          int num = vet[j];
          vet[j] = vet[i];
          vet[i] = num;
        }
    }
}
