#include <stdint.h>

/*
* Interrupt Descriptor Structure
*/
typedef struct {
   uint16_t   baseLow;
   uint16_t   selector;
   uint8_t   reservedIst;
   uint8_t   flags;
   uint16_t   baseMid;
   uint32_t   baseHigh;
   uint32_t   reserved;
} __attribute__((packed)) idtEntry;

/*
* Interrupt Descriptor Pointer
*/
typedef struct {
   uint16_t   limit;
   uint64_t   base;
} __attribute__((packed)) idtPointer;

/*
* Pushed Registers for ISR's
*/
typedef struct {
    uint64_t ds;
    uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
    uint64_t intNo, errCode;
    uint64_t rip, cs, eflags, useresp, ss;
} registers;

/*
* Prototypes
*/
void idtStart(void);
void idtSet(uint8_t, uint64_t, uint16_t, uint8_t);
void isrHandler(registers);
extern void _load_idt();
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();


/* Setup Table and Pointer */
idtEntry idt[256];
idtPointer idtP;

void idtStart(void) {
   /* Set IDT Pointer */
   idtP.limit = (sizeof(idtEntry) * 256) - 1;
   idtP.base = (uint64_t)&idt;
   
   /* Clear Memory for IDT's */
   memset((uint8_t *)&idt, 0, sizeof(idtEntry) * 256);
   
   /* Set IDT Gates */
   idtSet(0, (uint64_t)isr0, 0x08, 0x8E);
   idtSet(1, (uint64_t)isr1, 0x08, 0x8E);
   idtSet(2, (uint64_t)isr2, 0x08, 0x8E);
   idtSet(3, (uint64_t)isr3, 0x08, 0x8E);
   idtSet(4, (uint64_t)isr4, 0x08, 0x8E);
   idtSet(5, (uint64_t)isr5, 0x08, 0x8E);
   idtSet(6, (uint64_t)isr6, 0x08, 0x8E);
   idtSet(7, (uint64_t)isr7, 0x08, 0x8E);
   idtSet(8, (uint64_t)isr8, 0x08, 0x8E);
   idtSet(9, (uint64_t)isr9, 0x08, 0x8E);
   idtSet(10, (uint64_t)isr10, 0x08, 0x8E);
   idtSet(11, (uint64_t)isr11, 0x08, 0x8E);
   idtSet(12, (uint64_t)isr12, 0x08, 0x8E);
   idtSet(13, (uint64_t)isr13, 0x08, 0x8E);
   idtSet(14, (uint64_t)isr14, 0x08, 0x8E);
   idtSet(15, (uint64_t)isr15, 0x08, 0x8E);
   idtSet(16, (uint64_t)isr16, 0x08, 0x8E);
   idtSet(17, (uint64_t)isr17, 0x08, 0x8E);
   idtSet(18, (uint64_t)isr18, 0x08, 0x8E);
   idtSet(19, (uint64_t)isr19, 0x08, 0x8E);
   idtSet(20, (uint64_t)isr20, 0x08, 0x8E);
   idtSet(21, (uint64_t)isr21, 0x08, 0x8E);
   idtSet(22, (uint64_t)isr22, 0x08, 0x8E);
   idtSet(23, (uint64_t)isr23, 0x08, 0x8E);
   idtSet(24, (uint64_t)isr24, 0x08, 0x8E);
   idtSet(25, (uint64_t)isr25, 0x08, 0x8E);
   idtSet(26, (uint64_t)isr26, 0x08, 0x8E);
   idtSet(27, (uint64_t)isr27, 0x08, 0x8E);
   idtSet(28, (uint64_t)isr28, 0x08, 0x8E);
   idtSet(29, (uint64_t)isr29, 0x08, 0x8E);
   idtSet(30, (uint64_t)isr30, 0x08, 0x8E);
   idtSet(21, (uint64_t)isr31, 0x08, 0x8E);
   
   /* Load IDT Table */
   _load_idt();
}

void idtSet(uint8_t number, uint64_t base, uint16_t selector, uint8_t flags) {
   /* Set Base Address */
   idt[number].baseLow = base & 0xFFFF;
   idt[number].baseMid = (base >> 16) & 0xFFFF;
   idt[number].baseHigh = (base >> 32) & 0xFFFFFFFF;
   
   /* Set Selector */
   idt[number].selector = selector;
   idt[number].flags = flags;
   
   /* Set Reserved Areas to Zero */
   idt[number].reservedIst = 0;
   idt[number].reserved = 0;
}

void isrHandler(registers regs) {

}