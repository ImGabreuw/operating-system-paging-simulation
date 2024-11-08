#ifndef LOGICAL_MEMORY
#define LOGICAL_MEMORY
#define PAGE_SIZE 4096


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

Page* create_pg(Page *pg,int pageNumber, int processID);

void markDirty(Page *p);

void markClean(Page *p);

//LOGICAL MEMORY
typedef struct {
    int size;
    int pageSize;
    Page** pages;
}LogicalMemory;

LogicalMemory* create_lm(LogicalMemory* lm,int size, int pageSize);

Page* getPage(LogicalMemory* lm, int pageNumber);

Page* createPages(LogicalMemory* lm, int processSize);



#endif