__attribute__((noreturn))
void isr_handler(void)
{
    /* TODO: Add logging later */

    for (;;)
        __asm__ __volatile__ ("hlt");
}
