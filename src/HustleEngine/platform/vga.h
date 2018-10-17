#ifndef VGA_H
#define VGA_H

#include "common/platform.h"

void enter_m13h(void);
void leave_m13h(void);
void set_palette(buffer_t *palette);

#endif