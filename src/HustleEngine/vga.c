#include "src/hustle~1/vga.h"
#include "src/hustle~1/platform.h"
#include <dos.h>

#define PALETTE_INDEX       0x03c8
#define PALETTE_DATA        0x03c9
#define INPUT_STATUS        0x03da

/*
 * Enters mode 13h
 *
 * 256 indexed colour, 320x200
*/
void enter_m13h(void)
{
    union REGS in, out;

    in.h.ah = 0;
    in.h.al = 0x13;
    int86(0x10, &in, &out);
}

/*
 * Enters 80x25 text mode
*/
void leave_m13h(void)
{
    union REGS in, out;

    in.h.ah = 0;
    in.h.al = 0x03;
    int86(0x10, &in, &out);
}

/*
 * Sets mode 13h
 * 256 colour palette
*/
void set_palette(buffer_t *palette)
{
    int i;

    outp(PALETTE_INDEX, 0);     /* tell the VGA that palette data is coming. */

    for(i = 0; i < 256 * 3; ++i)
        outp(PALETTE_DATA, palette[i]);
}