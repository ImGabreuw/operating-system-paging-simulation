#include "page.h"

#include <stdlib.h>

int page_create(Page *page, int page_number, int pid)
{
    if (page == NULL)
        return -1;

    page->page_number = page_number;
    page->pid = pid;
    page->is_dirty = false;
    page->is_loaded = false;

    return EXIT_SUCCESS;
}

void page_mark_dirty(Page *page)
{
    if (page != NULL)
    {
        page->is_dirty = true;
    }
}

void page_mark_clean(Page *page)
{
    if (page != NULL)
    {
        page->is_dirty = false;
    }
}
