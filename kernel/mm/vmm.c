#include <stdint.h>
#include <stddef.h>
#include "../limine.h"
#include "../sys/print.h"
#include "pmm.h"

#define PAGE_SIZE 4096
#define NUM_PML4_ENTRIES 512
#define NUM_PDPT_ENTRIES 512
#define NUM_PD_ENTRIES 512
#define NUM_PT_ENTRIES 512

typedef struct {
    void* entries[NUM_PT_ENTRIES];
} PageTable;

typedef struct {
    PageTable* entries[NUM_PD_ENTRIES];
} PageDirectory;

typedef struct {
    PageDirectory* entries[NUM_PDPT_ENTRIES];
} PageDirectoryPointerTable;

typedef struct {
    PageDirectoryPointerTable* entries[NUM_PML4_ENTRIES];
} PageMapLevel4;

PageMapLevel4* pml4;

void VMMInit(struct limine_memmap_entry** memmap, size_t num_entries) {
    pml4 = (PageMapLevel4*)allocate(1);
    for (int i = 0; i < NUM_PML4_ENTRIES; i++) {
        pml4->entries[i] = NULL;
    }

    for (int i = 0; i < num_entries; i++) {
        if (memmap[i]->type == LIMINE_MEMMAP_USABLE) {
            print("Found Usable Memmap.\n");
            uint64_t start_addr = memmap[i]->base;
            uint64_t end_addr = memmap[i]->base + memmap[i]->length;

            for (uint64_t addr = start_addr; addr < end_addr; addr += PAGE_SIZE) {
                VMMMapPage((void*)addr, (void*)addr);
            }
        }
    }
}

void VMMMapPage(void* virtual_addr, void* physical_addr) {
    uint64_t vaddr = (uint64_t)virtual_addr;
    uint64_t paddr = (uint64_t)physical_addr;

    uint64_t pml4_index = (vaddr >> 39) & 0x1FF;
    uint64_t pdpt_index = (vaddr >> 30) & 0x1FF;
    uint64_t pd_index = (vaddr >> 21) & 0x1FF;
    uint64_t pt_index = (vaddr >> 12) & 0x1FF;

    if (pml4->entries[pml4_index] == NULL) {
        pml4->entries[pml4_index] = (PageDirectoryPointerTable*)allocate(1);
        for (int i = 0; i < NUM_PDPT_ENTRIES; i++) {
            pml4->entries[pml4_index]->entries[i] = NULL;
        }
    }

    if (pml4->entries[pml4_index]->entries[pdpt_index] == NULL) {
        pml4->entries[pml4_index]->entries[pdpt_index] = (PageDirectory*)allocate(1);
        for (int i = 0; i < NUM_PD_ENTRIES; i++) {
            pml4->entries[pml4_index]->entries[pdpt_index]->entries[i] = NULL;
        }
    }

    if (pml4->entries[pml4_index]->entries[pdpt_index]->entries[pd_index] == NULL) {
        pml4->entries[pml4_index]->entries[pdpt_index]->entries[pd_index] = (PageTable*)allocate(1);
        for (int i = 0; i < NUM_PT_ENTRIES; i++) {
            pml4->entries[pml4_index]->entries[pdpt_index]->entries[pd_index]->entries[i] = NULL;
        }
    }

    pml4->entries[pml4_index]->entries[pdpt_index]->entries[pd_index]->entries[pt_index] = (void*)(paddr | 0x03);
}

void VMMUnmapPage(void* virtual_addr) {
    uint64_t vaddr = (uint64_t)virtual_addr;

    uint64_t pml4_index = (vaddr >> 39) & 0x1FF;
    uint64_t pdpt_index = (vaddr >> 30) & 0x1FF;
    uint64_t pd_index = (vaddr >> 21) & 0x1FF;
    uint64_t pt_index = (vaddr >> 12) & 0x1FF;

    if (pml4->entries[pml4_index] == NULL) {
        return;
    }

    if (pml4->entries[pml4_index]->entries[pdpt_index] == NULL) {
        return;
    }

    if (pml4->entries[pml4_index]->entries[pdpt_index]->entries[pd_index] == NULL) {
        return;
    }

    pml4->entries[pml4_index]->entries[pdpt_index]->entries[pd_index]->entries[pt_index] = NULL;
}