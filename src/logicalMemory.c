#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logicalMemory.h"

//Metodos de Page
Page* create_pg(Page *pg,int pageNumber, int processID){
    pg->pageNumber = pageNumber;
    pg->processID = processID;
    pg->isDirty = false;
    pg->isLoaded = false;
    return pg;
}

void markDirty(Page *p){
    p->isDirty = true;
}

void markClean(Page *p){
    p->isDirty = false;
}


//Metodos de LogicalMemory

LogicalMemory* create_lm(LogicalMemory* lm,int size, int pageSize){
    lm->size = size;
    lm->pageSize = pageSize;
    lm->pages = (Page**)malloc(sizeof(Page*));
    return lm;
}

Page* getPage(LogicalMemory* lm, int pageNumber){
    for (int page_idx = 0; page_idx < lm->size; page_idx++){
        if(lm->pages[page_idx]->pageNumber == pageNumber) return lm->pages[page_idx];
    }
    return NULL;
}


Page* createPages(LogicalMemory* lm, int processSize){

    int pagesNeeded = (processSize + PAGE_SIZE - 1) / PAGE_SIZE; 

    lm->pages = (Page**) malloc(pagesNeeded * sizeof(Page*));  
    lm->size = pagesNeeded;  

    for (int i = 0; i < pagesNeeded; i++) {
        Page* page = (Page*) malloc(sizeof(Page));  
        page->pageNumber = i;                       
        page->isDirty = false;                      
        page->frameNumber = -1;                    

        lm->pages[i] = page;  
    }

    return lm->pages[0];  
}


