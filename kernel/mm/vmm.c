#include <stdint.h>
#include <stddef.h>
#include "vmm.h"
#include "pmm.h"
#include "../sys/print.h"

#define PAGE_SIZE 4096

#define PTE_PRESENT 0x1
#define PTE_WRITABLE 0x2
#define PTE_USER 0x4

#define PDE_PRESENT 0x1
#define PDE_WRITABLE 0x2
#define PDE_USER 0x4
#define PDE_4KB 0x80

#define PDPTE_PRESENT 0x1
#define PDPTE_WRITABLE 0x2
#define PDPTE_USER 0x4
#define PDPTE_1GB 0x80

typedef uint64_t pagemap_t;

typedef struct PageTable {
    uint64_t entries[512];
} PageTable;

PageTable* active_table;

typedef struct PageDirectoryPointerTable {
    uint64_t entries[512];
} PageDirectoryPointerTable;

PageDirectoryPointerTable* current_pdpt_table;

typedef struct PageDirectory {
    uint64_t entries[512];
} PageDirectory;

PageDirectory* current_pd_table;

typedef struct PageMapLevel4 {
    uint64_t entries[512];
} PageMapLevel4;

PageMapLevel4* current_pml4_table;

void switch_page_table(PageTable* table) {
    active_table = table;
    asm volatile("mov %0, %%cr3" :: "r"(table->entries));
}

void map_page(PageTable* table, uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags) {
    uint64_t pte_index = (virtual_addr >> 12) & 0x1FF;
    table->entries[pte_index] = (physical_addr & 0xFFFFFFFFFFFFF000) | flags;
}

void unmap_page(PageTable* table, uint64_t virtual_addr) {
    uint64_t pte_index = (virtual_addr >> 12) & 0x1FF;
    table->entries[pte_index] = 0;
}

PageTable* allocate_page_table(size_t num_pages) {
    void* table_addr = allocate(num_pages);

    if (table_addr == NULL) {
        print("NULL returned from allocation. Halting...\n");

        asm ("cli");
        for (;;) {
            asm ("hlt");
        }
    }

    PageTable* table = (PageTable*)table_addr;
    for (int i = 0; i < 512; i++) {
        table->entries[i] = 0;
    }

    return table;
}

void deallocate_page_table(PageTable* table, size_t num_pages) {
    deallocate(table, num_pages);
}

void VMMInit() {
    PageMapLevel4* pml4_table = allocate_page_table(1);
    if (pml4_table == NULL) {
        print("Page Map Level 4 table is NULL. Halting...\n");

        asm ("cli");
        for (;;) {
            asm ("hlt");
        }
    }

    PageDirectoryPointerTable* pdpt_table = allocate_page_table(1);
    if (pdpt_table == NULL) {
        print("Page Directory Pointer Table is NULL. Halting...\n");

        asm ("cli");
        for (;;) {
            asm ("hlt");
        }
    }

    PageDirectory* pd_table = allocate_page_table(1);
    if (pd_table == NULL) {
        print("Page Directory table is NULL. Halting...\n");

        asm ("cli");
        for (;;) {
            asm ("hlt");
        }
    }

    PageTable* pt_table = allocate_page_table(1);
    if (pt_table == NULL) {
        print("Page Table is NULL. Halting...\n");

        asm ("cli");
        for (;;) {
            asm ("hlt");
        }
    }

    pml4_table->entries[0] = (uint64_t)pdpt_table | PDPTE_PRESENT | PDPTE_WRITABLE | PDPTE_USER;
    pdpt_table->entries[0] = (uint64_t)pd_table | PDE_PRESENT | PDE_WRITABLE | PDE_USER;
    pd_table->entries[0] = (uint64_t)pt_table | PDE_PRESENT | PDE_WRITABLE | PDE_USER;

    current_pml4_table = pml4_table;
    current_pdpt_table = pdpt_table;
    current_pd_table = pd_table;
    active_table = pt_table;

    switch_page_table((PageTable*)pt_table);
}

void switch_page_directory(PageDirectory* directory) {
    current_pd_table = directory;
    switch_page_table((PageTable*)directory);
}
