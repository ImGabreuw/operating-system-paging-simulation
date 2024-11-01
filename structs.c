#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FRAME_SIZE (4096)

typedef struct{
    int pid;
    int *virtual_addresses;
    int v_addresses_amount;
    PageTable *pageTable;
    int size;
} Process;


typedef struct {
    bool isBusy;
    Process *process;
    Page *page;
    char *data;

}Frame;

typedef struct{
    Process *process;
    int page_id;
    char *data;
}Page;

typedef struct{
    void **pages;
    Process *process;
}PageTable;





int main(int argc, char const *argv[])
{
    
    return 0;
}
