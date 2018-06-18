#ifndef FILESYS_H
#define FILESYS_H

#include "src/hustle~1/platform.h"

buffer_t *load_bmp_image(const char *file);
buffer_t *load_bmp_palette(const char *file);

#endif