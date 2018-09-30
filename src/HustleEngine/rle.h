#ifndef RLE_H
#define RLE_H

typedef struct {
    byte col;
    uint length;
} RLEChunk; 

typedef struct {
    uint bg_len;
    uint fg_len;  
} MonoRLEChunk; 

typedef RLEChunk     far RLEImage;
typedef MonoRLEChunk far RLEImageMono;

RLEImage *buffer_to_rle(buffer_t *buf);
MonoRLEChunk *monochrome_buffer_to_rle(buffer_t *buf);

#endif /* RLE_H */
