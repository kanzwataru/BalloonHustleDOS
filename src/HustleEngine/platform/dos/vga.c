#include "platform/video.h"
#include "common/platform.h"
#include <dos.h>

#define PALETTE_INDEX       0x03c8 /* vga palette register */
#define PALETTE_DATA        0x03c9
#define INPUT_STATUS_0      0x03da /* used for querying vblank */

enum VGA_VIDEO_MODES {
    VGA_CHUNKY256 = 0x13, /* 256 colour indexed chained,  320x200 non-square pixels */
    VGA_PLANAR16  = 0x12, /* 16  colour planar unchained, 640x400 square pixels */
    VGA_PLANAR16HALF = 0x12, /* 16 colour planar unchained, 640x200 non-square (2:1) pixels */
    VGA_TEXT80COL = 0x03  /* 80 column text mode, 80x25 */
};

/*
 * Switch VGA display mode
 * 
 * (uses VGA_VIDEO_MODES not video.h VIDEO_MODES)
*/
static void vga_modeset(byte mode)
{
    union REGS in, out;

    in.h.ah = 0;
    in.h.al = mode;
    int86(0x10, &in, &out);
}

/*
 * Video API init
*/
void video_init_mode(byte mode, int scaling)
{
    /* we ONLY support Mode 13h for now!! */
    if(mode != VIDEO_MODE_LOW256)
        NOT_IMPLEMENTED;
    
    vga_modeset(VGA_CHUNKY256);
}

/*
 * Video API exit
*/
void video_exit(void)
{
    vga_modeset(VGA_TEXT80COL);
}

/*
 * Block and wait for vsync
*/
void video_wait_vsync(void)
{
    while(inportb(INPUT_STATUS_0) & 8) {;}
    while(!(inportb(INPUT_STATUS_0) & 8)) {;}
}

/*
 * Sets mode 13h
 * 256 colour palette
*/
void video_set_palette(buffer_t *palette)
{
    int i;

    outp(PALETTE_INDEX, 0);     /* tell the VGA that palette data is coming. */

    for(i = 0; i < 256 * 3; ++i)
        outp(PALETTE_DATA, palette[i]);
}