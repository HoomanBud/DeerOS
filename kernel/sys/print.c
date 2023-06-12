#include "./../limine.h"
#include "./../flanterm/backends/fb.h"
#include "print.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct flanterm_context* context = NULL;
struct limine_framebuffer* framebuffer2 = NULL;

size_t strlen2(const char *str) {
    size_t i = 0;
    while(str[i]) ++i;
    return i;
}

void print(char* str)
{
    if (framebuffer2 == NULL)
   {
      framebuffer2 = framebuffer_request.response->framebuffers[0];
   }

   if (context == NULL)
   {
      context = flanterm_fb_init(NULL, NULL, framebuffer2->address, framebuffer2->width, framebuffer2->height, framebuffer2->pitch, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 1, 1, 1, 0);
   }

    flanterm_write(context, str, strlen2(str));
}