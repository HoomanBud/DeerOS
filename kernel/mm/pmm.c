#include <stdint.h>
#include <stddef.h>
#include "../limine.h"

typedef struct FMemRegion {
    uint64_t start;
    uint64_t size;
    struct FMemRegion* next;
} FMemRegion;

FMemRegion* freelist = NULL;

#define PAGE_SIZE 4096

void FreelistPMMInit(struct limine_memmap_entry* mem_map, size_t mem_map_entries) {
    for (size_t i = 0; i < mem_map_entries; i++) {
        struct limine_memmap_entry* entry = &mem_map[i];
        if (entry->type == 1) {
            uint64_t start_page = (entry->base + PAGE_SIZE - 1) / PAGE_SIZE;
            uint64_t end_page = entry->base + entry->length;
            end_page /= PAGE_SIZE;

            for (uint64_t page = start_page; page < end_page; page++) {
                FMemRegion* region = (FMemRegion*)(page * PAGE_SIZE);
                region->start = page * PAGE_SIZE;
                region->size = PAGE_SIZE;
                region->next = freelist;
                freelist = region;
            }
        }
    }
}

void* allocate_page() {
    if (freelist != NULL) {
        FMemRegion* allocated_page = freelist;
        freelist = freelist->next;
        return (void*)allocated_page->start;
    }

    return NULL;
}

void deallocate_page(void* page) {
    FMemRegion* region = (FMemRegion*)page;
    region->start = (uint64_t)page;
    region->size = PAGE_SIZE;
    region->next = freelist;
    freelist = region;
}