#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "process_structures.h"

//Metodos de PageTableEntry

PageTableEntry* create_pte(PageTableEntry* pte,int pageNumber, int frameNumber){
    pte->pageNumber = pageNumber;
    pte->frameNumber = frameNumber;
    pte->isDirty = false;
    pte->isValid = false;
    return pte;
}

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
PageTable* create_pta(PageTable* pta, int numberOfPages){
    pta->entries = (PageTableEntry**)malloc(sizeof(PageTableEntry*));

}

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
    
    //nao encontrou espaco
    return false;

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

//Metodos de Processo
Process* create_p(int pid, int firstAddress, int lastAddress, int size, PageTable* pg, LogicalMemory* lm ){
    Process *p;
    p->pid = pid;
    p->size = size;
    p->pageTable = pg;
    p->logicalMemory = lm;
    p->addressesCount = lastAddress-firstAddress;
    for(int address_idx = 0; address_idx < p->addressesCount; address_idx++){
        p->accessSequence[address_idx] = firstAddress+address_idx;
    }

}