#include <stdint.h>
#include <stddef.h>
#include "../sys/print.h"
#include "../limine.h"

static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

#define KERNEL_VIRTUAL_OFFSET kernel_address_request.response->virtual_base
#define KERNEL_PHYSICAL_OFFSET kernel_address_request.response->physical_base

#define PAGE_SIZE 4096
#define NUM_MAP_LEVELS 4

typedef struct {
    uint64_t entry;
} PageTableEntry;

typedef struct {
    PageTableEntry entries[512];
} PageTable;

PageTable* pml4_table;

void VMMInit() {
    pml4_table = (PageTable*)allocate(1);
    for (size_t i = 0; i < 512; i++) {
        pml4_table->entries[i].entry = 0;
    }

    PageTable* pdpt_table = (PageTable*)allocate(1);
    for (size_t i = 0; i < 512; i++) {
        pdpt_table->entries[i].entry = 0;
    }
    pml4_table->entries[0].entry = ((uint64_t)pdpt_table - KERNEL_PHYSICAL_OFFSET) | 0x3;

    PageTable* pd_table = (PageTable*)allocate(1);
    for (size_t i = 0; i < 512; i++) {
        pd_table->entries[i].entry = 0;
    }
    pdpt_table->entries[0].entry = ((uint64_t)pd_table - KERNEL_PHYSICAL_OFFSET) | 0x3;

    PageTable* pt_table = (PageTable*)allocate(1);
    for (size_t i = 0; i < 512; i++) {
        pt_table->entries[i].entry = 0;
    }
    pd_table->entries[0].entry = ((uint64_t)pt_table - KERNEL_PHYSICAL_OFFSET) | 0x3;

    uint64_t physical_addr = allocate(1) * PAGE_SIZE;
    uint64_t virtual_addr = physical_addr + KERNEL_VIRTUAL_OFFSET;

    map_page(virtual_addr, physical_addr);
}


void map_page(uint64_t virtual_addr, uint64_t physical_addr) {
    PageTable* pdpt_table;
    PageTable* pd_table;
    PageTable* pt_table;

    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    uint64_t pdpt_index = (virtual_addr >> 30) & 0x1FF;
    uint64_t pd_index = (virtual_addr >> 21) & 0x1FF;
    uint64_t pt_index = (virtual_addr >> 12) & 0x1FF;

    if (!pml4_table->entries[pml4_index].entry) {
        pml4_table->entries[pml4_index].entry = (uint64_t)allocate(1);
        pdpt_table = (PageTable*)(pml4_table->entries[pml4_index].entry);
        for (size_t i = 0; i < 512; i++) {
            pdpt_table->entries[i].entry = 0;
        }
    } else {
        pdpt_table = (PageTable*)(pml4_table->entries[pml4_index].entry);
    }

    if (!pdpt_table->entries[pdpt_index].entry) {
        pdpt_table->entries[pdpt_index].entry = (uint64_t)allocate(1);
        pd_table = (PageTable*)(pdpt_table->entries[pdpt_index].entry);
        for (size_t i = 0; i < 512; i++) {
            pd_table->entries[i].entry = 0;
        }
    } else {
        pd_table = (PageTable*)(pdpt_table->entries[pdpt_index].entry);
    }

    if (!pd_table->entries[pd_index].entry) {
        pd_table->entries[pd_index].entry = (uint64_t)allocate(1);
        pt_table = (PageTable*)(pd_table->entries[pd_index].entry);
        for (size_t i = 0; i < 512; i++) {
            pt_table->entries[i].entry = 0;
        }
    } else {
        pt_table = (PageTable*)(pd_table->entries[pd_index].entry);
    }

    pt_table->entries[pt_index].entry = physical_addr | 0x3;
}

void unmap_page(uint64_t virtual_addr) {
    PageTable* pdpt_table;
    PageTable* pd_table;
    PageTable* pt_table;

    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    uint64_t pdpt_index = (virtual_addr >> 30) & 0x1FF;
    uint64_t pd_index = (virtual_addr >> 21) & 0x1FF;
    uint64_t pt_index = (virtual_addr >> 12) & 0x1FF;

    if (!pml4_table->entries[pml4_index].entry) {
        return;
    } else {
        pdpt_table = (PageTable*)(pml4_table->entries[pml4_index].entry);
    }

    if (!pdpt_table->entries[pdpt_index].entry) {
        return;
    } else {
        pd_table = (PageTable*)(pdpt_table->entries[pdpt_index].entry);
    }

    if (!pd_table->entries[pd_index].entry) {
        return;
    } else {
        pt_table = (PageTable*)(pd_table->entries[pd_index].entry);
    }

    pt_table->entries[pt_index].entry = 0;
}
