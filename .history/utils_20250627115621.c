#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "utils.h"

void embaralhar(FILE *arq, int total_records){
  if (total_records <= 1) return;
  
  srand(time(NULL));
  
  for (int i = 0; i < total_records - 1; i++) {

    int j = i + rand() / (RAND_MAX / (total_records - i) + 1);

    fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *reg_i = le_funcionario(arq);

    fseek(arq, j * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *reg_j = le_funcionario(arq);

    fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
    salva_funcionario(reg_j, arq);

    fseek(arq, j * tamanho_registro_funcionario(), SEEK_SET);
    salva_funcionario(reg_i, arq);

    free(reg_i);
    free(reg_j);
  }
  fflush(arq);
}