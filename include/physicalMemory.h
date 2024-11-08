#ifndef PHISYCAL_MEMORY
#define PHYSYCAL_MEMORY

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "logicalMemory.h"

#define FRAME_SIZE (4096)

//FRAME
typedef struct{
    int frameNumber;
    int pageNumber;
    int frameSize;
    char* data;
    bool isOccupied;

}Frame;

Frame* create_fr(Frame* fr,int frameNumber, int frameSize, char* data);

void loadPage(Page* p, Frame* f);

void unloadPage(Frame *f);

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

PhysicalMemory* create_pm(PhysicalMemory* pm,int size);


Frame* allocateFrame(PhysicalMemory* pm);

int getFreeFrameCount(PhysicalMemory* pm);

bool releaseFrame(PhysicalMemory* pm,int  frameNumber);

bool writeToFrame(PhysicalMemory* pm,int frameNumber, char *data);

char* readFromFrame(PhysicalMemory* pm, int frameNumber);

#endif