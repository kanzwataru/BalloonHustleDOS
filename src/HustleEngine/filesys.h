#ifndef FILESYS_H
#define FILESYS_H

#include "src/hustle~1/types.h"

void load_bmp_image(buffer_t *buf, const char *file);
buffer_t *load_bmp_palette(const char *file);

#endif