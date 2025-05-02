#include <stdio.h>
#include <limine.h>
#include <ewx/console.h>

#include "gdt/gdt.h"
#include "kernel.h"

/* Limine global framebuffer structure */
struct limine_framebuffer *fb = NULL;

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

__attribute__((aligned(16))) static uint8_t kernel_stack[4096];  /* 4 KiB aligned stack */
uint8_t *_kernel_stack_top = kernel_stack + sizeof(kernel_stack);

static psf1_font_t *_load_psf_driver(void)
{
    psf1_font_t *font = psf1_init();
    if (!font)
        halt();

    kputs("[ OK ] PSF1 Initialized!", 0, 0, WHITE, font);
    return font;
}

void _start(void) {
    if (!LIMINE_BASE_REVISION_SUPPORTED) halt();

    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count < 1) {

        halt();
    }

    fb = framebuffer_request.response->framebuffers[0];

    /* Load all drivers & gdt & idt .. */
    //gdt_init();

    _load_psf_driver();
    /***********************/

    halt();
}
