#include "idt.h"
#include "stdint.h"

typedef struct {
    uint16_t base_low;
    uint16_t selector;

    uint8_t zero;
    uint8_t flags;

    uint16_t base_mid;
    uint32_t base_high;
    uint32_t pad;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t    limit;
    uint64_t    base;
} __attribute__((packed)) idtr_t;

typedef struct regs * (*interrupt_handler_t)(struct regs *);

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10 ();
extern void isr11 ();
extern void isr12 ();
extern void isr13 ();
extern void isr14 ();
extern void isr15 ();
extern void isr16 ();
extern void isr17 ();
extern void isr18 ();
extern void isr19 ();
extern void isr20 ();
extern void isr21 ();
extern void isr22 ();
extern void isr23 ();
extern void isr24 ();
extern void isr25 ();
extern void isr26 ();
extern void isr27 ();
extern void isr28 ();
extern void isr29 ();
extern void isr30 ();
extern void isr31 ();


extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10 ();
extern void irq11 ();
extern void irq12 ();
extern void irq13 ();
extern void irq14 ();
extern void irq15 ();

static idt_entry_t idt[256];
static idtr_t idtr;

extern void _load_idt(idtr_t *);

void init_idt()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt) * 256 - 1;
    idtr.limit = sizeof(idt_entry_t) * 256 -1;
    idtr.base  = (uint64_t)&idt;

    memset(idt, 0, sizeof(idt_entry_t)*256);

    idt_set_gate(0,  isr0,  0x28, 0x8E, 0);
    idt_set_gate(1,  isr1,  0x28, 0x8E, 0);
    idt_set_gate(2,  isr2,  0x28, 0x8E, 0);
    idt_set_gate(3,  isr3,  0x28, 0x8E, 0);
    idt_set_gate(4,  isr4,  0x28, 0x8E, 0);
    idt_set_gate(5,  isr5,  0x28, 0x8E, 0);
    idt_set_gate(6,  isr6,  0x28, 0x8E, 0);
    idt_set_gate(7,  isr7,  0x28, 0x8E, 0);
    idt_set_gate(8,  isr8,  0x28, 0x8E, 0);
    idt_set_gate(9,  isr9,  0x28, 0x8E, 0);
    idt_set_gate(10, isr10, 0x28, 0x8E, 0);
    idt_set_gate(11, isr11, 0x28, 0x8E, 0);
    idt_set_gate(12, isr12, 0x28, 0x8E, 0);
    idt_set_gate(13, isr13, 0x28, 0x8E, 0);
    idt_set_gate(14, isr14, 0x28, 0x8E, 0);
    idt_set_gate(15, isr15, 0x28, 0x8E, 0);
    idt_set_gate(16, isr16, 0x28, 0x8E, 0);
    idt_set_gate(17, isr17, 0x28, 0x8E, 0);
    idt_set_gate(18, isr18, 0x28, 0x8E, 0);
    idt_set_gate(19, isr19, 0x28, 0x8E, 0);
    idt_set_gate(20, isr20, 0x28, 0x8E, 0);
    idt_set_gate(21, isr21, 0x28, 0x8E, 0);
    idt_set_gate(22, isr22, 0x28, 0x8E, 0);
    idt_set_gate(23, isr23, 0x28, 0x8E, 0);
    idt_set_gate(24, isr24, 0x28, 0x8E, 0);
    idt_set_gate(25, isr25, 0x28, 0x8E, 0);
    idt_set_gate(26, isr26, 0x28, 0x8E, 0);
    idt_set_gate(27, isr27, 0x28, 0x8E, 0);
    idt_set_gate(28, isr28, 0x28, 0x8E, 0);
    idt_set_gate(29, isr29, 0x28, 0x8E, 0);
    idt_set_gate(30, isr30, 0x28, 0x8E, 0);
    idt_set_gate(31, isr31, 0x28, 0x8E, 0);
   //  idt_set_gate(32, (uint64_t)irq0, 0x08, 0x8E);
   //  idt_set_gate(33, (uint64_t)irq1, 0x08, 0x8E);
   //  idt_set_gate(34, (uint64_t)irq2, 0x08, 0x8E);
   //  idt_set_gate(35, (uint64_t)irq3, 0x08, 0x8E);
   //  idt_set_gate(36, (uint64_t)irq4, 0x08, 0x8E);
   //  idt_set_gate(37, (uint64_t)irq5, 0x08, 0x8E);
   //  idt_set_gate(38, (uint64_t)irq6, 0x08, 0x8E);
   //  idt_set_gate(39, (uint64_t)irq7, 0x08, 0x8E);
   //  idt_set_gate(40, (uint64_t)irq8, 0x08, 0x8E);
   //  idt_set_gate(41, (uint64_t)irq9, 0x08, 0x8E);
   //  idt_set_gate(42, (uint64_t)irq10, 0x08, 0x8E);
   //  idt_set_gate(43, (uint64_t)irq11, 0x08, 0x8E);
   //  idt_set_gate(44, (uint64_t)irq12, 0x08, 0x8E);
   //  idt_set_gate(45, (uint64_t)irq13, 0x08, 0x8E);
   //  idt_set_gate(46, (uint64_t)irq14, 0x08, 0x8E);
   //  idt_set_gate(47, (uint64_t)irq15, 0x08, 0x8E);

   _load_idt(&idtr);
}

void idt_set_gate(uint8_t num, interrupt_handler_t handler, uint16_t selector, uint8_t flags, int userspace) {
    uintptr_t base = (uintptr_t)handler;
    idt[num].base_low  = (base) & 0xFFFF;
    idt[num].base_mid  = (base >> 16) & 0xFFFF;
    idt[num].base_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].pad = 0;
    idt[num].flags = flags | (userspace ? 0x60 : 0);
}