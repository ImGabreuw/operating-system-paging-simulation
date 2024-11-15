#include "process_queue.h"

#include <stdlib.h>

#include "log.h"

void queue_init(Queue *queue)
{
    queue->front = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

bool queue_is_empty(Queue *queue)
{
    return queue->size == 0;
}

int queue_enqueue(Queue *queue, Process *process)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    if (!new_node)
    {
        log_message(LOG_ERROR, "Falha ao alocar memória para o nó da fila.");
        return EXIT_FAILURE;
    }

    new_node->process = process;
    new_node->next = NULL;

    if (queue_is_empty(queue))
    {
        queue->front = queue->tail = new_node;
    }
    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    queue->size++;
    return EXIT_SUCCESS;
}

Process *queue_dequeue(Queue *queue)
{
    if (queue_is_empty(queue))
        return NULL;

    Node *temp = queue->front;
    Process *process = temp->process;

    queue->front = temp->next;
    if (!queue->front)
        queue->tail = NULL;

    free(temp);
    queue->size--;
    return process;
}

void queue_clear(Queue *queue)
{
    Node *current = queue->front;

    while (current)
    {
        Node *next = current->next;
        free(current->process);
        free(current);
        current = next;
    }
    
    queue->front = queue->tail = NULL;
    queue->size = 0;
}
