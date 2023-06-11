#include <stdint.h>
#include "gdt.h"

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

// Define the GDT descriptor
typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_descriptor_t;

// Define the GDT
gdt_entry_t gdt[3];
gdt_descriptor_t gdt_desc;

// Function to load the GDT
void load_gdt() {
    gdt_desc.limit = sizeof(gdt) - 1;
    gdt_desc.base = (uint64_t)&gdt;

    asm volatile (
        "lgdt %0"
        :
        : "m"(gdt_desc)
    );
}

// Function to set up a GDT entry
void set_gdt_entry(uint32_t index, uint64_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_mid = (base >> 16) & 0xFF;
    gdt[index].access = access;
    gdt[index].granularity = granularity;
    gdt[index].base_high = (base >> 24) & 0xFF;
    gdt[index].base_high |= (base >> 32) & 0xFFFFFFFFFFUL;
}

// Example usage
void setup_gdt() {
    // Set up null segment
    set_gdt_entry(0, 0, 0, 0, 0);

    // Set up code segment
    set_gdt_entry(1, 0, 0xFFFFF, 0x9A, 0xA);

    // Set up data segment
    set_gdt_entry(2, 0, 0xFFFFF, 0x92, 0xA);

    // Load the GDT
    load_gdt();
}