#include "isr.h"
#include "idt.h"

isr_handler_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t interrupt, isr_handler_t handler)
{
    interrupt_handlers[interrupt] = handler;
}

void isr_handler(registers_t regs)
{
    if(regs.int_no == GENERAL_PROTECTION_FAULT)
    {
        
    }

    if(interrupt_handlers[regs.int_no])
    {
        interrupt_handlers[regs.int_no](regs);
    }
}