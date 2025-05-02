#ifndef PSF1_H_
#define PSF1_H_

#include <stdint.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct {
    uint8_t magic[2];
    uint8_t mode;
    uint8_t charsize;
} __attribute__((packed)) psf1_header_t;

typedef struct {
    psf1_header_t *header;
    void *glyphbuf;
} psf1_font_t;

psf1_font_t *psf1_init(void);

#endif /* PSF1_H_ */
