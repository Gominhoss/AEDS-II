#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "log.h"

static LARGE_INTEGER inicio, frequencia;

void iniciar_medicao() {
    QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&inicio);
}

double finalizar_medicao() {
    LARGE_INTEGER fim;
    QueryPerformanceCounter(&fim);
    return (double)(fim.QuadPart - inicio.QuadPart) * 1000.0 / frequencia.QuadPart;  // milissegundos
}

void registrar_tempo_execucao(const char *descricao, double tempo) {
    FILE *log = fopen("log_execucao.txt", "a"); // abre em modo append
    if (log == NULL) {
        perror("Erro ao abrir o arquivo de log");
        return;
    }

    printf("Vou escrever no log...\n");
    fprintf(log, "%s: %.6f milisegundos\n", descricao, tempo);
    printf("Escrevi no log!\n");  
    fflush(log); // garante que os dados sejam escritos imediatamente
    fclose(log);
}
