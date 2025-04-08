#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>
#include <limine.h>

#include <xv4/colors.h>
#include <xv4/psf1.h>

#define BITS_PER_BYTE 8

void kputchar(uint8_t c, int x, int y,
    color_t color, psf1_font_t *font);

void kputs(const char *s, int x, int y,
    color_t color, psf1_font_t *font);

#endif /* CONSOLE_H_ */
