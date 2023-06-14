#include "../sys/print.h"
#include "../sys/idt.h"
#include "pit.h"

int timer_ticks = 0;

void timer_handler(struct regs *r)
{
    timer_ticks++;

    if (timer_ticks % 18 == 0)
    {
        print("One second has passed\n");
    }
}

void timer_install()
{
    irq_install(0, timer_handler);
}