#include <stddef.h>

void FreelistPMMInit(struct limine_memmap_entry* memmap, size_t num_entries);
void* allocate(size_t size);
void deallocate(void* region, size_t size);