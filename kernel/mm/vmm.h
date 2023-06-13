#ifndef VMM_H
#define VMM_H

#include <stdint.h>
#include <stddef.h>
#include "../limine.h"

void VMMInit(struct limine_memmap_entry** memmap, size_t num_entries);
void VMMMapPage(void* virtual_addr, void* physical_addr);
void VMMUnmapPage(void* virtual_addr);

void* allocate(size_t num_pages);
void deallocate(void* page, size_t num_pages);

#endif