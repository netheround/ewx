#include <stddef.h>
#include <xv4/psf1.h>

extern uint8_t _binary_font_psf_start[];
extern uint8_t _binary_font_psf_end;

psf1_font_t *psf1_init(void) {
    /*
    * TODO: Later use calloc and malloc once libc is ported.
    * Currently, we're using a static font structure.
    */

    psf1_header_t *header = (psf1_header_t*)_binary_font_psf_start;

    if ((header->magic[0] == PSF1_MAGIC0)
        && header->magic[1] == PSF1_MAGIC1) {

        uint8_t *glyph = _binary_font_psf_start + sizeof(psf1_header_t);

        static psf1_font_t font;
        font.header = header;
        font.glyphbuf = glyph;

        return &font;
    }

    return NULL;
}
