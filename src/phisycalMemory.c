#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "physicalMemory.h"

#define FRAME_SIZE (4096)

//Metodos de Frame
Frame* create_fr(Frame* fr,int frameNumber, int frameSize, char* data){
    fr->frameNumber = frameNumber;
    fr->pageNumber = -1; //Nao ocupado
    fr->frameSize = frameSize;
    fr->isOccupied = false;
    strcpy(fr->data,data);
    return fr;
}

char* getData(Frame *f){
    return f->data;
}

void setData(Frame *f, char* data){
    strcpy(f->data,data);
}

void clear(Frame *f){
    f->data = realloc(&f->data, f->frameSize);
}

void loadPage(Page* p, Frame* f){
    f->pageNumber = p->pageNumber;
}

void unloadPage(Frame* f){
    f->pageNumber = -1;
}

//Metodos de Physical Memory
PhysicalMemory* create_pm(PhysicalMemory* pm,int size){
    pm->size = size;
    pm->frameSize = FRAME_SIZE;
    return pm;
}

Frame* allocateFrame(PhysicalMemory* pm) {
    if (pm->freeFrameCount <= 0) {
        return NULL;  
    }

    // Percorre os quadros para encontrar um livre
    for (int i = 0; i < pm->size; i++) {
        if (!pm->frames[i]->isOccupied) {  
            pm->frames[i]->isOccupied = true;     
            pm->frames[i]->data = (char*)malloc(FRAME_SIZE); 
            memset(pm->frames[i]->data, 0, FRAME_SIZE);       

            pm->freeFrameCount--;  
            return pm->frames[i];  
        }
    }

    return NULL;  
}

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
