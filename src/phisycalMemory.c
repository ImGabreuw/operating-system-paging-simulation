#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "physicalMemory.h"

#define FRAME_SIZE (4096)

//Metodos de Frame

char* getData(Frame *f){
    return f->data;
}

void setData(Frame *f, char* data){
    strcpy(f->data,data);
}

void clear(Frame *f){
    f->data = realloc(&f->data, f->frameSize);
}

//Metodos de Physical Memory

//TODO: Implementar
Frame* allocateFrame(PhysicalMemory* pm);

int getFreeFrameCount(PhysicalMemory* pm){
    return pm->freeFrameCount;
}

bool releaseFrame(PhysicalMemory* pm,int  frameNumber){
    for(int frame_idx = 0; frame_idx < pm->size; frame_idx++){
        if(pm->frames[frame_idx]->frameNumber == frameNumber){
            pm->frames[frame_idx]->isOccupied = false;
            return true;
        }
    }
    return false;
}

bool writeToFrame(PhysicalMemory* pm,int frameNumber, char *data){
    for(int frame_idx = 0; frame_idx < pm->size; frame_idx++){
        if(pm->frames[frame_idx]->frameNumber == frameNumber){
            setData(&pm->frames[frame_idx],data);
            return true;
        }
    }
    return false;
}

char* readFromFrame(PhysicalMemory* pm, int frameNumber){
    for(int frame_idx = 0; frame_idx < pm->size; frame_idx++){
        if(pm->frames[frame_idx]->frameNumber == frameNumber)
            return pm->frames[frame_idx]->data;
    }
    return NULL;
}
