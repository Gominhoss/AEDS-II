/**
 * @file log.c
 * @brief Implementação das funções para medição de tempo e registro de logs.
 */

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "log.h"

// Variáveis estáticas para armazenar o início da contagem e a frequência do contador de performance.
static LARGE_INTEGER inicio, frequencia;

/**
 * @brief Inicia a medição de tempo.
 * Captura a frequência do contador de performance do hardware e o ponto de tempo inicial.
 */
void iniciar_medicao() {
    QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&inicio);
}

/**
 * @brief Finaliza a medição de tempo e retorna o tempo decorrido em milissegundos.
 * @return O tempo decorrido desde a chamada de iniciar_medicao() em milissegundos.
 */
double finalizar_medicao() {
    LARGE_INTEGER fim;
    QueryPerformanceCounter(&fim);
    return (double)(fim.QuadPart - inicio.QuadPart) * 1000.0 / frequencia.QuadPart;  // Retorna em milissegundos
}

/**
 * @brief Registra uma mensagem com o tempo de execução em um arquivo de log.
 * @param descricao Uma string que descreve a operação que foi medida.
 * @param tempo O tempo de execução em milissegundos a ser registrado.
 */
void registrar_tempo_execucao(const char *descricao, double tempo) {
    // Abre o arquivo de log em modo de anexação (append)
    FILE *log = fopen("log/log_execucao.txt", "a"); 
    if (log == NULL) {
        perror("Erro ao abrir o arquivo de log");
        return;
    }

    // Escreve a descrição e o tempo no arquivo
    fprintf(log, "%s: %.6f milisegundos\n", descricao, tempo); 
    
    // Garante que os dados sejam escritos imediatamente no disco
    fflush(log); 
    fclose(log);
}