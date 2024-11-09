#include "frame.h"

#include <stdlib.h>
#include <string.h>

#include "log.h"

int frame_create(Frame *frame, int frame_number, int frame_size)
{
    if (frame == NULL)
    {
        log_message(LOG_ERROR, "Invalid frame pointer. Please check the line where the Frame structure was allocated.");
        return EXIT_FAILURE;
    }

    if (frame_number < -1)
    {
        log_message(LOG_ERROR, "Frame number must be positive integers or -1. Check the parameters passed to frame_create().");
        return EXIT_FAILURE;
    }

    frame->frame_number = frame_number;

    if (frame_size <= 0)
    {
        log_message(LOG_ERROR, "Size must be positive integers. Check the parameters passed to frame_create().");
        return EXIT_FAILURE;
    }

    frame->size = frame_size;

    frame->data = (char *)malloc(frame_size * sizeof(char));

    if (frame->data == NULL)
    {
        log_message(LOG_ERROR, "Memory allocation failed for frame data. This could indicate a memory leak or other resource issue. Please check the system resources by running './run.sh -d' with Valgrind.");
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
