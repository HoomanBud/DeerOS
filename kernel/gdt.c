#include <stdint.h>
#include <gdt.h>

typedef struct{
    uint16_t limit;
    uint64_t base;
}__attribute__((packed)) gdt_register_t;

uint64_t gdt[] = {
  0,
  0x00af9b000000ffff,
  0x00af93000000ffff
};

gdt_register_t gdt_reg;

void gdt_i()
{
    gdt_reg.base = (uint64_t)&gdt;
    gdt_reg.limit = sizeof(gdt) - 1;

    asm volatile (
        "lgdt (%0);"
        "pushq $0x08;"
        "pushq $1f;"
        "lretq;"
        "1:"
        "mov $0x10, %%eax;"
        "mov %%eax, %%ds;"
        "mov %%eax, %%es;"
        "mov %%eax, %%fs;"
        "mov %%eax, %%gs;"
        "mov %%eax, %%ss;"
        :
        : "r"(&gdt_reg)
        : "rax", "memory"
    );
}