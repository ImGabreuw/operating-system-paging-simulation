#ifndef PHISYCAL_MEMORY
#define PHYSYCAL_MEMORY

#include <stdio.h>
#include <stdbool.h>

//FRAME
typedef struct{
    int frameNumber;
    int frameSize;
    char* data;
    bool isOccupied;

}Frame;

char* getData(Frame *f);

void setData(Frame *f, char* data);

void clear(Frame *f);


//PHYSICAL MEMORY
typedef struct{
    int size;
    int frameSize;
    Frame** frames;
    int freeFrameCount;
    int accessDelay;

}PhysicalMemory;

//TODO: Implementar
Frame* allocateFrame(PhysicalMemory* pm);

int getFreeFrameCount(PhysicalMemory* pm);

bool releaseFrame(PhysicalMemory* pm,int  frameNumber);

bool writeToFrame(PhysicalMemory* pm,int frameNumber, char *data);

char* readFromFrame(PhysicalMemory* pm, int frameNumber);

#endif