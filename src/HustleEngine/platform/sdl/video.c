/*
 * SDL1.2 backend Video module
*/
#include "platform/video.h"
#include <SDL/SDL.h>
#include <stdio.h>

#define LOW256_WIDTH        320
#define LOW256_HEIGHT       200

#define HIGH16_WIDTH        640
#define HIGH16_HEIGHT       480

#define HIGH16HALF_WIDTH    640
#define HIGH16HALF_HEIGHT   200

static SDL_Surface *screen;   /* screen surface (scaled) */
static SDL_Surface *framebuffer; /* based on render.c buffer, at original resolution*/
static SDL_Rect     fbrect = {0, 0, 0, 0};
static byte scale;
static byte video_mode;

/*
 * Video API init
*/
void video_init_mode(byte mode, int scaling)
{
    scale = scaling;
    video_mode = mode;
    switch(mode) {
        case VIDEO_MODE_LOW256:
            fbrect.w = LOW256_WIDTH;
            fbrect.h = LOW256_HEIGHT;
            break;
        case VIDEO_MODE_HIGH16:
            fbrect.w = HIGH16_WIDTH;
            fbrect.h = HIGH16_HEIGHT;
            break;
        case VIDEO_MODE_HIGH16HALF:
            fbrect.w = HIGH16HALF_WIDTH;
            fbrect.h = HIGH16HALF_HEIGHT;
            break;
        default:
            PANIC("Invalid video mode");
            break;
    }
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL Initialization Failed \n\n%s\n", SDL_GetError());
        return 1;
    }
    
    screen = SDL_SetVideoMode(fbrect.w, fbrect.h, 8, SDL_SWSURFACE | SDL_ANYFORMAT);
    if(!screen) {
        fprintf(stderr, "Failed to set video mode (%d x %d)\n%s\n", fbrect.w, fbrect.h, SDL_GetError());
        return 1;
    }
}

/*
 * Video API exit
*/
void video_exit(void)
{
    SDL_FreeSurface(framebuffer);
    SDL_Quit();
}

/*
 * Block and wait for vsync
*/
void video_wait_vsync(void)
{
    
}

/*
 * Video API flip buffer
 *
 * We have to do a bit o' finangling to get the VGA mode 13h stuff to draw on an SDL surface
*/
void video_flip(buffer_t *backbuf)
{
    
}

/*
 * Sets mode 13h
 * 256 colour palette
*/
void video_set_palette(buffer_t *palette)
{
    
}
