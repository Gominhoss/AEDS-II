#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "utils.h"

void embaralhar(int *vet, size_t vetSize){
    time_t t;
    srand((unsigned) time(&t));

    if(vetSize > 1){
        size_t i;
        for(i = 0; i < vetSize - 1; i++){
          size_t j = i + rand() / (RAND_MAX / (vetSize - i) + 1);
          int num = vet[j];
          vet[j] = vet[i];
          vet[i] = num;

        }
    }
}