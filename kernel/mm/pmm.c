#include <stdint.h>
#include <stddef.h>
#include "../limine.h"

typedef struct FMemRegion {
    uint64_t start;
    uint64_t size;
    struct FMemRegion* next;
} FMemRegion;

FMemRegion* freelist = NULL;

void FreelistPMMInit(struct limine_memmap_entry *mem_map, size_t mem_map_entries) {
    for (size_t i = 0; i < mem_map_entries; i++) {
        struct limine_memmap_entry* entry = &mem_map[i];
        if (entry->type == 0) {
            FMemRegion* region = (FMemRegion*)entry->base;
            region->start = entry->base;
            region->size = entry->length;
            region->next = freelist;
            freelist = region;
        }
    }
}

void* allocate(size_t size) {
    FMemRegion* prev = NULL;
    FMemRegion* current = freelist;
    while (current != NULL) {
        if (current->size >= size) {
            if (prev == NULL) {
                freelist = current->next;
            } else {
                prev->next = current->next;
            }
            return (void*)current->start;
        }
        prev = current;
        current = current->next;
    }

    return NULL;
}

void deallocate(void* ptr, size_t size) {
    FMemRegion* region = (FMemRegion*)ptr;
    region->start = (uint64_t)ptr;
    region->size = size;
    region->next = freelist;
    freelist = region;
}