#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include "process.h"

typedef struct Node
{
    Process *process;
    struct Node *next;
} Node;

typedef struct
{
    Node *front; // Primeiro elemento da fila
    Node *tail;  // Último elemento da fila
    int size;    // Número de elementos na fila
} Queue;

/**
 * Inicializa a fila.
 *
 * @param queue Ponteiro para a estrutura da fila.
 */
void queue_init(Queue *queue);

/**
 * Verifica se a fila está vazia.
 *
 * @param queue Ponteiro para a estrutura da fila.
 * @return true se a fila estiver vazia, false caso contrário.
 */
bool queue_is_empty(Queue *queue);

/**
 * Adiciona um processo ao final da fila.
 *
 * @param queue Ponteiro para a estrutura da fila.
 * @param process Ponteiro para o processo a ser adicionado.
 * @return 0 para sucesso ou 1 para erro
 */
int queue_enqueue(Queue *queue, Process *process);

/**
 * Remove e retorna o processo do início da fila.
 *
 * @param queue Ponteiro para a estrutura da fila.
 * @return Ponteiro para o processo removido, ou NULL se a fila estiver vazia.
 */
Process *queue_dequeue(Queue *queue);

/**
 * Limpa a fila e libera todos os nós e processos alocados.
 *
 * @param queue Ponteiro para a estrutura da fila.
 */
void queue_clear(Queue *queue);

#endif // QUEUE_H
