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


