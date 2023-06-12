#include <stdint.h>
#include <stddef.h>
#include "vmm.h"
#include "pmm.h"

#define PAGE_SIZE 4096

#define PTE_PRESENT 0x1
#define PTE_WRITABLE 0x2
#define PTE_USER 0x4

#define PDE_PRESENT 0x1
#define PDE_WRITABLE 0x2
#define PDE_USER 0x4
#define PDE_4MB 0x80

typedef uint64_t pagemap_t;

typedef struct PageTable {
    uint64_t entries[512];
} PageTable;

PageTable* active_table;

typedef struct PageDirectory {
    uint64_t entries[512];
} PageDirectory;

PageDirectory* current_directory;

void switch_page_table(PageTable* table) {
    active_table = table;
    asm volatile("mov %0, %%cr3" ::"r"(&table->entries));
}

void map_page(PageTable* table, uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags) {
    uint64_t pte_index = (virtual_addr >> 12) & 0x1FF;
    table->entries[pte_index] = (physical_addr & 0xFFFFF000) | flags;
}

void unmap_page(PageTable* table, uint64_t virtual_addr) {
    uint64_t pte_index = (virtual_addr >> 12) & 0x1FF;
    table->entries[pte_index] = 0;
}

PageTable* allocate_page_table(size_t size) {
    size_t num_pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    void* table_addr = allocate(num_pages * PAGE_SIZE);
    if (table_addr == NULL) {
        return NULL;
    }

    PageTable* table = (PageTable*)table_addr;
    for (int i = 0; i < 512; i++) {
        table->entries[i] = 0;
    }

    return table;
}

void deallocate_page_table(PageTable* table, size_t size) {
    size_t num_pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    deallocate(table, num_pages * PAGE_SIZE);
}

void VMMInit() {
    PageDirectory* directory = allocate_page_table(4096);
    if (directory == NULL) {
        return;
    }

    map_page(directory, (uint64_t)directory, (uint64_t)directory, PDE_PRESENT | PDE_WRITABLE);

    current_directory = directory;
    switch_page_table((PageTable*)directory);
}

void switch_page_directory(PageDirectory* directory) {
    current_directory = directory;
    switch_page_table((PageTable*)directory);
}

// void kernel_main(struct memory_map_entry* mem_map, size_t mem_map_entries) {
//     // Initialize the Virtual Memory Manager
//     init_vmm();

//     // Example usage: Map a virtual address to a physical address
//     uint64_t virtual_addr = 0x12345000;
//     uint64_t physical_addr = 0x56789000;
//     map_page(active_table, virtual_addr, physical_addr, PTE_PRESENT | PTE_WRITABLE | PTE_USER);

//     // Example usage: Access the mapped address
//     uint64_t* value = (uint64_t*)virtual_addr;
//     *value = 42;

//     // Example usage: Unmap the virtual address
//     unmap_page(active_table, virtual_addr);

//     // End of the kernel code
//     while (1) {
//         // Do nothing
//     }
// }
