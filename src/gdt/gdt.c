#include "gdt.h"

extern void _load_gdt(gdt_ptr_t *gdt);

static gdt_entry_t gdt[GDT_ENTRY_COUNT];
static gdt_tss_entry_t tss_descriptor;
static tss_t tss;

static gdt_ptr_t gdt_descriptor;

static void gdt_set_entry(int index, uint32_t base, uint32_t limit,
    uint8_t access, uint8_t flags)
{
    gdt_entry_t *entry = &gdt[index];
    entry->limit_low = limit & 0xFFFF;
    entry->base_low = base & 0xFFFF;
    entry->base_mid = (base >> 16) & 0xFF;
    entry->access = access;
    entry->granularity = ((limit >> 16) & 0x0F) | (flags & 0xF0);
    entry->base_high = (base >> 24) & 0xFF;
}

static void gdt_set_tss(uint64_t base, uint32_t limit)
{
    tss_descriptor.limit_low = limit & 0xFFFF;
    tss_descriptor.base_low = base & 0xFFFF;
    tss_descriptor.base_mid1 = (base >> 16) & 0xFF;
    tss_descriptor.access = 0x89;
    tss_descriptor.flags_limit = ((limit >> 16) & 0x0F) | 0x00;
    tss_descriptor.base_mid2 = (base >> 24) & 0xFF;
    tss_descriptor.base_high = (base >> 32) & 0xFFFFFFFF;
    tss_descriptor.reserved = 0;
}

void gdt_init(void)
{
    gdt_set_entry(0, 0, 0, 0, 0);       /* Null descriptor */

    gdt_set_entry(1, 0, 0, 0x9A, 0xA0); /* Ring 0 Code Segment */
    gdt_set_entry(2, 0, 0, 0x92, 0xA0); /* Ring 0 Data Segment */

    gdt_set_entry(3, 0, 0, 0xFA, 0xA0); /* Ring 3 Code Segment */
    gdt_set_entry(4, 0, 0, 0xF2, 0xA0); /* Ring 3 Data Segment */

    for (uintptr_t i = 0; i < sizeof(tss); ++i)
        ((uint8_t*)&tss)[i] = 0;

    extern uint8_t _kernel_stack_top[];
    tss.rsp0 = (uint64_t)_kernel_stack_top;

    gdt_set_tss((uint64_t)&tss, sizeof(tss));
    *((gdt_tss_entry_t*)&gdt[5]) = tss_descriptor;

    gdt_descriptor.size = sizeof(gdt) - 1;
    gdt_descriptor.offset = (uint64_t)&gdt;

    _load_gdt(&gdt_descriptor);
}
