/**
 * @file log.h
 * @brief Protótipos de funções para medição de tempo e registro de logs.
 *
 * Este arquivo define a interface para um conjunto de funções utilitárias
 * que permitem medir o tempo de execução de blocos de código e registrar
 * essas medições em um arquivo de log.
 */

#ifndef LOG_H
#define LOG_H

/**
 * @brief Registra uma mensagem com o tempo de execução em um arquivo de log.
 * O arquivo de log é "log/log_execucao.txt" e é aberto em modo de anexação (append).
 * @param descricao Uma string que descreve a operação que foi medida.
 * @param tempo O tempo de execução em milissegundos.
 */
void registrar_tempo_execucao(const char *descricao, double tempo);

/**
 * @brief Finaliza a medição de tempo e retorna o tempo decorrido.
 * Calcula a diferença entre o tempo atual e o tempo registrado por iniciar_medicao().
 * @return O tempo decorrido em milissegundos.
 */
double finalizar_medicao();

/**
 * @brief Inicia a medição de tempo.
 * Armazena o ponto de tempo atual para ser usado posteriormente por finalizar_medicao().
 */
void iniciar_medicao();

#endif