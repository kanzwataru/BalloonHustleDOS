#include "src/hustle~1/rle.h"

RLEImage *buffer_to_rle(buffer_t *buf)
{
    NOT_IMPLEMENTED
}

RLEImageMono *monochrome_buffer_to_rle(buffer_t *buf, int width, int height, byte fgcol, byte bgcol)
{
    uint offset = 0, line = 0;
    MonoRLEChunk far *p;
    MonoRLEChunk far *rle;

    rle = farcalloc((width * height) * 2, sizeof(MonoRLEChunk));
    p = rle;
    assert(rle);

    printf("RLE time");

    while(offset < width * height) {
        while(line < width) {
            assert(buf[offset + line] == bgcol && buf[offset + line] == fgcol); /* catch broken sprites */
            while(buf[offset + line] == bgcol) {
                ++p->bglen;
                ++line;

                if(line == width)
                    goto next;
            }
            while(buf[offset + line] == fgcol) {
                ++p->fglen;
                ++line;

                if(line == width)
                    goto next;
            }

            ++p;
        }

    next:
        offset += line;
        line = 0;
        ++p;
    }

    return rle;
}

