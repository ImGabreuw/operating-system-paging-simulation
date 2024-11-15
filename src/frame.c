#include "frame.h"

#include <stdlib.h>
#include <string.h>

#include "log.h"

int frame_create(Frame *frame, int frame_number, int frame_size)
{
    if (frame == NULL)
    {
        log_message(LOG_ERROR, "Invalid frame pointer. Please check the line where the Frame structure was allocated.");
        log_message(LOG_ERROR, "Ponteiro para a estrutura de quadro inválido. Por favor, verifique a linha onde foi alocada a estrutura Frame.");
        return EXIT_FAILURE;
    }

    if (frame_number < -1)
    {
        log_message(LOG_ERROR, "Número do quadro deve ser números positivos ou -1. Verifique os parâmetros passados para frame_create().");
        return EXIT_FAILURE;
    }

    frame->frame_number = frame_number;

    if (frame_size <= 0)
    {
        log_message(LOG_ERROR, "O tamanho deve ser números positivos. Verifique os parâmetros passados para frame_create().");
        return EXIT_FAILURE;
    }

    frame->size = frame_size;

    frame->data = (char *)malloc(frame_size * sizeof(char));

    if (frame->data == NULL)
    {
        log_message(LOG_ERROR, "A alocação de memória falhou para os dados do quadro. Isso pode indicar um vazão de memória ou outra questão de recurso. Por favor, verifique os recursos do sistema executando './run.sh -d' com Valgrind.");
        return EXIT_FAILURE;
    }

    frame->is_occupied = false;

    return EXIT_SUCCESS;
}

char *get_data(Frame *frame)
{
    return frame->data;
}

void set_data(Frame *frame, char *data)
{
    if (frame->data)
    {
        strcpy(frame->data, data);
    }
}

void clear_frame(Frame *frame)
{
    if (frame->data)
    {
        free(frame->data);
    }
    frame->data = NULL;
    frame->is_occupied = false;
}
