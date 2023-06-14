#include "print.h"
#include "../flanterm/flanterm.h"
#include "../flanterm/backends/fb.h"
#include "../limine.h"

size_t strlen3(const char *str) {
    size_t i = 0;
    while(str[i]) ++i;
    return i;
}

void rsodInit(char error)
{
    asm("cli");

    struct limine_framebuffer* fb = getFramebuffer();

    for (uint64_t i = 0; i < fb->height; i++)
    {
       for (uint64_t e = 0; e < fb->width; e++)
       {
           uintptr_t base = (uintptr_t) fb->address;
           *(uint32_t*) (base + i * fb->pitch + e * fb->bpp / 8) = 0xb25757;
       }
    }

    struct flanterm_context* context = getContext();

    context->set_cursor_pos(context, 25, 50);
    flanterm_write(context, "Well that's fucked up, Look's like your PC ran into an issue.", strlen3("Well that's fucked up, Look's like your PC ran into an issue."));
    context->set_cursor_pos(context, 25, 53);
    flanterm_write(context, error, strlen3(error));

    for (;;)
    {
        asm("hlt");
    }
}