#include <stdint.h>
#include <stddef.h>
#include "../limine.h"
#include "pmm.h"
#include "../sys/print.h"

typedef struct {
    uint64_t start_addr;
    size_t num_pages;
} FreeMemoryRegion;

#define PAGE_SIZE 4096
#define MAX_FREE_REGIONS 256

FreeMemoryRegion free_regions[MAX_FREE_REGIONS];
size_t num_free_regions = 0;

void add_free_region(uint64_t start_addr, size_t num_pages) {
    if (num_free_regions >= MAX_FREE_REGIONS) {
        print("Max free regions reached. Halting...\n");
    
        asm ("cli");
        for (;;)
        {
            asm ("hlt");
        }
    }

    free_regions[num_free_regions].start_addr = start_addr;
    free_regions[num_free_regions].num_pages = num_pages;
    num_free_regions++;
}

void FreelistPMMInit(struct limine_memmap_entry** memmap, size_t num_entries) {
    for (size_t i = 0; i < num_entries; i++) {
        if (memmap[i]->type == LIMINE_MEMMAP_USABLE) {
            uint64_t start_addr = memmap[i]->base;
            uint64_t end_addr = memmap[i]->base + memmap[i]->length;
            size_t num_pages = (end_addr - start_addr) / PAGE_SIZE;
            
            // printf("SA: %d\n", start_addr);
            // printf("EA: %d\n", end_addr);
            // printf("NP: %d\n", num_pages);

            add_free_region(start_addr, num_pages);
        }
    }
}

void* allocate(size_t num_pages) {
    for (size_t i = 0; i < num_free_regions; i++) {
        if (free_regions[i].num_pages >= num_pages) {
            uint64_t start_addr = free_regions[i].start_addr;
            free_regions[i].start_addr += num_pages * PAGE_SIZE;
            free_regions[i].num_pages -= num_pages;
            return (void*)start_addr;
        }
    }

    print("NULL returned from allocation. Halting...\n");
    
    asm ("cli");
    for (;;)
    {
        asm ("hlt");
    }
}

void deallocate(void* page, size_t num_pages) {
    uint64_t addr = (uint64_t)page;
    size_t region_index = 0;

    for (size_t i = 0; i < num_free_regions; i++) {
        if (addr > free_regions[i].start_addr) {
            region_index = i;
        } else {
            break;
        }
    }

    if (region_index > 0 && addr == free_regions[region_index - 1].start_addr + free_regions[region_index - 1].num_pages * PAGE_SIZE) {
        free_regions[region_index - 1].num_pages += num_pages;
    } else if (region_index < num_free_regions - 1 && addr + num_pages * PAGE_SIZE == free_regions[region_index + 1].start_addr) {
        free_regions[region_index].num_pages += num_pages + free_regions[region_index + 1].num_pages;

        for (size_t i = region_index + 1; i < num_free_regions - 1; i++) {
            free_regions[i] = free_regions[i + 1];
        }
        num_free_regions--;
    } else {
        for (size_t i = num_free_regions; i > region_index; i--) {
            free_regions[i] = free_regions[i - 1];
        }
        free_regions[region_index].start_addr = addr;
        free_regions[region_index].num_pages = num_pages;
        num_free_regions++;
    }
}
