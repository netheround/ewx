#include <stdbool.h>

#include "idt.h"

static bool vectors[IDT_MAX_DESCRIPTORS];
extern void *isr_stub_table[];

__attribute__((aligned(0x10)))
static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;

void set_idt_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low         = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs       = 0x08;
    descriptor->ist             = 0;
    descriptor->attributes      = flags;
    descriptor->isr_mid         = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high        = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved        = 0;
}

void idt_init(void)
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; ++vector) {
        set_idt_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ __volatile__ ("lidt %0" : : "m"(idtr));
    __asm__ __volatile__ ("sti");
}
