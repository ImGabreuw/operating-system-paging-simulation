#ifndef LOGICAL_MEMORY
#define LOGICAL_MEMORY


#include <stdio.h>
#include <stdbool.h>

//PAGE
typedef struct {
    int pageNumber;
    int frameNumber;
    int processID;
    bool isDirty;
    bool isLoaded;
}Page;

void markDirty(Page *p);

void markClean(Page *p);

void loadPage(Page* p, int frameNumber);

void unloadPage(Page* p);

//LOGICAL MEMORY
typedef struct {
    int size;
    int pageSize;
    Page** pages;
}LogicalMemory;

Page* getPage(LogicalMemory* lm, int pageNumber);

//TODO: Implementar
Page* createPages(LogicalMemory* lm, int processSize);

#endif