#include "page_table_entry.h"

#include "log.h"

int page_table_entry_create(PageTableEntry *page_table_entry, int page_number, int frame_number)
{
    if (!page_table_entry) {
        log_message(LOG_ERROR, "Invalid page table entry pointer.");
        return EXIT_FAILURE;
    }

    if (page_number < 0) {
        log_message(LOG_ERROR, "Page number must be non-negative integer.");
        return EXIT_FAILURE;
    }

    if (frame_number < -1) {
        log_message(LOG_ERROR, "Frame number must be non-negative integer or -1.");
        return EXIT_FAILURE;
    }

    page_table_entry->page_number = page_number;
    page_table_entry->frame_number = frame_number;
    page_table_entry->is_dirty = false;
    page_table_entry->is_valid = false;

    return EXIT_SUCCESS;
}

void pagetable_mark_dirty(PageTableEntry *p_entry)
{
    p_entry->is_dirty = true;
}

void pagetable_mark_clean(PageTableEntry *p_entry)
{
    p_entry->is_dirty = false;
}

void validate(PageTableEntry *p_entry)
{
    p_entry->is_valid = true;
}

void invalidate(PageTableEntry *p_entry)
{
    p_entry->is_valid = false;
}