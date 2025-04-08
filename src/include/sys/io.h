#ifndef IO_H_
#define IO_H_

#include <stdint.h>

static __inline uint8_t
inb (uint16_t __port)
{
    uint8_t _v;

    __asm__ __volatile__ ("inb %w1,%0":"=a" (_v):"Nd" (__port));
    return _v;
}

static __inline void
outb (uint8_t __value, uint16_t __port)
{
    __asm__ __volatile__ ("outb %b0,%w1": :"a" (__value), "Nd" (__port));
}

#endif /* IO_H_ */
