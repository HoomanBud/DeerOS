#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <flanterm/backends/fb.h>
#include <./sys/gdt.h>
#include <./sys/idt.h>
#include <kernel.h>
#include <./sys/print.h>
//#include <time/pit.h>
//#include <time/time.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <printf/printf.h>

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.




static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};


























void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

































size_t strlen(const char *str) {
    size_t i = 0;
    while(str[i]) ++i;
    return i;
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void) {
    // Ensure we got a framebuffer.
    // if (framebuffer_request.response == NULL
    //  || framebuffer_request.response->framebuffer_count < 1) {
    //     hcf();
    // }

    // Fetch the first framebuffer.
    // struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    // for (size_t i = 0; i < 100; i++) {
    //     uint32_t *fb_ptr = framebuffer->address;
    //     fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xdc5b57;
    // }

    // delay(5000);

    // for (size_t i = 0; i < 100; i++) {
    //     uint32_t *fb_ptr = framebuffer->address;
    //     fb_ptr[i * (framebuffer->pitch / 4) + i] = 0x107c10;
    // }

    //uint32_t memSize = 1024 + bootinfo->m_memoryLo + bootinfo->m_memoryHi*64;

    //struct limine_memmap_entry** memmap = memmap_request.response->entries[0];

    // struct flanterm_context *fc = flanterm_fb_init(NULL, NULL, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 1, 1, 1, 0);

    // flanterm_write(fc, "Loading DeerOS...\n", strlen("Loading DeerOS...\n"));
    // flanterm_write(fc, "Loading Graphical Mode...\n", strlen("Loading Graphical Mode...\n"));

    print("Loading DeerOS...\n");

    FreelistPMMInit(memmap_request.response->entries, memmap_request.response->entry_count);
    print("Physical Memory Manager Init: OK.\n");
    VMMInit();
    print("Virtual Memory Manager Init: OK\n");
    gdt_init();
    print("GDT Init: OK.\n");
    idtStart();
    print("IDT Init: OK.\n");

    // for (int i = 0; i < 10; i++)
    // {
    //     void* r = allocate(1);

    //     printf("%p\n", r);
    // }


    //for (uint64_t i = 0; i < framebuffer->height; i++)
    //{
    //    for (uint64_t e = 0; e < framebuffer->width; e++)
    //    {
    //        uint32_t color = 0x000000;
    //        uintptr_t base = (uintptr_t) framebuffer->address;
    //        *(uint32_t*) (base + i * framebuffer->pitch + e * framebuffer->bpp / 8) = color;
    //    }
    //}

    //asm("int $0x0");

    // We're done, just hang...
    hcf();
}
