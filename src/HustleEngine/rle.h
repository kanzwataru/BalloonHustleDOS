#ifndef RLE_H
#define RLE_H

#include "src/hustle~1/platform.h"

typedef struct {
    byte col;
    uint length;
} RLEChunk; 

typedef struct {
    uint bglen;
    uint fglen;  
} MonoRLEChunk; 

typedef RLEChunk     far RLEImage;
typedef MonoRLEChunk far RLEImageMono;

RLEImage *buffer_to_rle(buffer_t *buf);
RLEImageMono *monochrome_buffer_to_rle(buffer_t *buf, int width, int height, byte fgcol, byte bgcol);

#endif /* RLE_H */
