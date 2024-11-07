#include <stdio.h>
#include <stdbool.h>
#include "logicalMemory.h";

//Metodos de Page
void markDirty(Page *p){
    p->isDirty = true;
}

void markClean(Page *p){
    p->isDirty = false;
}

void loadPage(Page* p, int frameNumber){
    p->frameNumber = frameNumber;
}

void unloadPage(Page* p){
    p->frameNumber = -1;
}


//Metodos de LogicalMemory
Page* getPage(LogicalMemory* lm, int pageNumber){
    for (int page_idx = 0; page_idx < lm->size; page_idx++){
        if(lm->pages[page_idx]->pageNumber == pageNumber) return lm->pages[page_idx];
    }
    return NULL;
}


//TODO: Implementar
Page* createPages(LogicalMemory* lm, int processSize);


