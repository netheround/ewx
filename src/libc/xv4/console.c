#include <xv4/console.h>

extern struct limine_framebuffer *fb;

void kputchar(uint8_t c, int x, int y,
    color_t color, psf1_font_t *font)
{
    uint8_t *glyph = font->glyphbuf + c * font->header->charsize;

    for (int row = 0; row < font->header->charsize; ++row) {
        uint8_t line = glyph[row];

        for (int col = 0; col < BITS_PER_BYTE; ++col) {
            if ((line >> (7 - col)) & 1) {
                int px = x + col;
                int py = y + row;

                if (px >= 0 && px < (int)fb->width
                    && py >= 0 && py < (int)fb->height) {

                    uint32_t *fb_ptr = (uint32_t*)fb->address;
                    fb_ptr[py * (fb->pitch / 4) + px] = color;
                }
            }
        }
    }
}

void kputs(const char *s, int x, int y,
    color_t color, psf1_font_t *font)
{
    int x_dup = x;

    while (*s) {
        if (*s == '\n') {
            x = x_dup;
            y += font->header->charsize;
        } else {
            kputchar(*s, x, y, color, font);
            x += BITS_PER_BYTE;
        }
        s++;
    }
}
