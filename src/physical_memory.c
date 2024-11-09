#include "physical_memory.h"

#include <stdlib.h>
#include <string.h>

#include "log.h"

int physical_memory_create(PhysicalMemory *physical_memory, int size)
{
    if (size <= 0)
    {
        log_message(LOG_ERROR, "Invalid memory size. Must be greater than zero.");
        return EXIT_FAILURE;
    }

    physical_memory->size = size;

    if (!physical_memory || !size)
    {
        log_message(LOG_ERROR, "Invalid parameters for function physical_memory_create().");
        return EXIT_FAILURE;
    }

    physical_memory->frame_size = FRAME_SIZE;
    physical_memory->free_frame_count = size / FRAME_SIZE;

    physical_memory->frames = (Frame **)malloc(physical_memory->free_frame_count * sizeof(Frame *));

    if (!physical_memory->frames)
    {
        log_message(LOG_ERROR, "Failed to allocate memory for frames in physical memory.");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < physical_memory->free_frame_count; i++)
    {
        physical_memory->frames[i] = (Frame *)malloc(sizeof(Frame));

        if (!physical_memory->frames[i])
        {
            log_message(LOG_ERROR, "Failed to allocate memory for one of the frames in physical memory.");
            return EXIT_FAILURE;
        }

        frame_create(physical_memory->frames[i], i, FRAME_SIZE);
    }

    return EXIT_SUCCESS;
}

Frame *allocate_frame(PhysicalMemory *physical_memory)
{
    if (physical_memory->free_frame_count <= 0)
    {
        return NULL;
    }

    for (int i = 0; i < physical_memory->size / physical_memory->frame_size; i++)
    {
        if (!physical_memory->frames[i]->is_occupied)
        {
            physical_memory->frames[i]->is_occupied = true;
            physical_memory->free_frame_count--;
            return physical_memory->frames[i];
        }
    }

    return NULL;
}

int get_free_frame_count(PhysicalMemory *physical_memory)
{
    return physical_memory->free_frame_count;
}

int release_frame(PhysicalMemory *physical_memory, int frame_number)
{
    if (frame_number >= 0 && frame_number < physical_memory->size / physical_memory->frame_size)
    {
        physical_memory->frames[frame_number]->is_occupied = false;
        physical_memory->free_frame_count++;

        clear_frame(physical_memory->frames[frame_number]);

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int write_to_frame(PhysicalMemory *physical_memory, int frame_number, char *data)
{
    if (frame_number >= 0 && frame_number < physical_memory->size / physical_memory->frame_size)
    {
        set_data(physical_memory->frames[frame_number], data);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

char *read_from_frame(PhysicalMemory *physical_memory, int frame_number)
{
    if (frame_number >= 0 && frame_number < physical_memory->size / physical_memory->frame_size)
    {
        return get_data(physical_memory->frames[frame_number]);
    }

    return NULL;
}

void physical_memory_free_frames(PhysicalMemory *physical_memory) {
        for (int i = 0; i < physical_memory->free_frame_count; i++)
    {
        free(physical_memory->frames[i]);
    }

    free(physical_memory->frames);
}