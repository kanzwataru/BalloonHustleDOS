#include "src/hustle~1/filesys.h"
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <malloc.h>

struct FileLoadData {
    FILE *fp;
    uint col_num, width, height;
};

static void fskip(FILE *fp, int num_bytes)
{
    int i;
    for(i = 0; i < num_bytes; ++i)
        fgetc(fp);
}

static struct FileLoadData open_bmp_file(const char *file)
{
    struct FileLoadData d;

    /* open the file */
    if ((d.fp = fopen(file,"rb")) == NULL) {
        printf("Error opening file %s\n", file);
    }

    /* read in the width and height of the image, and the
    number of colors used; ignore the rest */
    fskip(d.fp,18);
    fread(&d.width, sizeof(uint), 1, d.fp);
    fskip(d.fp,2);
    fread(&d.height,sizeof(uint), 1, d.fp);
    fskip(d.fp,22);
    fread(&d.col_num,sizeof(uint), 1, d.fp);
    fskip(d.fp,6);

    /* assume we are working with an 8-bit file */
    if (d.col_num == 0) d.col_num = 256;

    return d;
}

/* 
 * http://www.brackeen.com/vga/source/djgpp20/palette.c.html
*/
void load_bmp_image(buffer_t *buf, const char *file)
{
    long i;
    int x;
    struct FileLoadData d = open_bmp_file(file);

    /* ignore palette */
    fskip(d.fp,d.col_num * 4);

    for(i = (d.height - 1) * d.width; i >= 0; i -= d.width)
        for(x = 0; x < d.width; ++x)
            buf[(uint)(i + x)] = (byte)fgetc(d.fp);
    
    fclose(d.fp);
}

buffer_t *load_bmp_palette(const char *file)
{
    long i;
    struct FileLoadData d = open_bmp_file(file);

    buffer_t *palette = farcalloc(256 * 3, sizeof(byte));

    for(i = 0; i < d.col_num; i+= 3) {
        palette[(int)(i + 2)] = fgetc(d.fp) >> 2;
        palette[(int)(i + 1)] = fgetc(d.fp) >> 2;
        palette[(int)(i + 0)] = fgetc(d.fp) >> 2;
        fgetc(d.fp);
    }

    fclose(d.fp);

    return palette;
}