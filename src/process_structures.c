#include <stdio.h>
#include <stdbool.h>
#include "process_structures.h"

//Metodos de PageTableEntry

void markDirty(PageTableEntry* p_entry){
    p_entry->isDirty=true;
}

void markClean(PageTableEntry* p_entry){
    p_entry->isDirty=false;
}

void validate(PageTableEntry* p_entry){
    p_entry->isValid=true;
}

void invalidate(PageTableEntry* p_entry){
    p_entry->isValid=false;
}

//Metodos de PageTable

bool addMapping(PageTable *pagetable, int pageNum, int frameNum){
    int entry_idx = 0;
    
    //Procurar o proximo espaco vazio
    while(entry_idx < pagetable->numberOfPages && pagetable->entries[entry_idx] != NULL) 
        entry_idx++;
    
    //Inserir o novo registro
    if(entry_idx < pagetable->numberOfPages){
        pagetable->entries[entry_idx]->frameNumber = frameNum;
        pagetable->entries[entry_idx]->frameNumber = pageNum;
        return true; 
    }
    
    return false; //nao encontrou espaco

}


bool removeMapping(PageTable *pagetable,int pageNum){

    //Procurar registro
    for(int entry_idx = 0; entry_idx < pagetable->entries; entry_idx++){
        if(pagetable->entries[entry_idx]->pageNumber == pageNum){
            pagetable->entries[entry_idx]->pageNumber = -1;
            pagetable->entries[entry_idx]->frameNumber = -1;
            return true; //encontrou e removeu
        }
    }
    return false; // nao encontrou
}

int getFrameNumber(PageTable *pagetable,int pageNum){
    //Procurar registro
    for(int entry_idx = 0; entry_idx < pagetable->entries; entry_idx++){
        if(pagetable->entries[entry_idx]->pageNumber == pageNum){
            return pagetable->entries[entry_idx]->frameNumber; //encontrou
        }
    }

    return -1; //nao encontrou
}

bool isPageValid(PageTable *pagetable,int pageNum){
    
    for(int entry_idx = 0; entry_idx < pagetable->entries; entry_idx++){
        if(pagetable->entries[entry_idx]->pageNumber == pageNum){
            return pagetable->entries[entry_idx]->isValid;
        }
    }
}