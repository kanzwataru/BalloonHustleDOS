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
static int scale;
static byte video_mode;

SDL_Color fbpalette[256];
/*
 * Video API init
*/
void video_init_mode(byte mode, byte scaling)
{
    assert(scaling > 0);
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
        exit(1);
    }
    
    screen = SDL_SetVideoMode(fbrect.w * scale, fbrect.h * scale, 24, SDL_SWSURFACE | SDL_ANYFORMAT);
    if(!screen) {
        fprintf(stderr, "Failed to set video mode (%d x %d)\n%s\n", fbrect.w * scale, fbrect.h * scale, SDL_GetError());
        exit(1);
    }
    
    /* framebuffer is 8-bit indexed (256 col) just like VGA mode 13h */
    framebuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, fbrect.w, fbrect.h, 8, 0,0,0,0);
    if(!framebuffer) {
        fprintf(stderr, "Failed to create framebuffer (%d x %d)\n%s\n", fbrect.w, fbrect.h, SDL_GetError());
        exit(1);
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
    SDL_Delay(12); /* fake vsync for now */
}

/*
 * Video API flip buffer
 *
 * We have to do a bit o' finangling to get the VGA mode 13h stuff to draw on an SDL surface
*/
void video_flip(buffer_t *backbuf)
{
    //printf("Video flip called\n");
    /* we ONLY support Mode 13h for now!! */
    if(video_mode != VIDEO_MODE_LOW256)
        NOT_IMPLEMENTED;
    
    if(SDL_MUSTLOCK(framebuffer))
        SDL_LockSurface(framebuffer);
        
    memcpy(framebuffer->pixels, backbuf, fbrect.w * fbrect.h);
    //*((buffer_t *)framebuffer->pixels) = 4;
    
    if(SDL_MUSTLOCK(framebuffer))
        SDL_UnlockSurface(framebuffer);
    
    SDL_BlitSurface(framebuffer, NULL, screen, NULL);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

/*
 * Sets colour palette
 *
 * Depending on the video mode it may be 16 or 256 colours
*/
void video_set_palette(buffer_t *palette)
{
    printf("palette set\n");
    assert(video_mode != 0);
    switch(video_mode) {
        case VIDEO_MODE_LOW256:
            for(int i = 0; i < 256; ++i) {
                /* left-shift to convert the VGA-compatible
                 * 6-bit colours back to 8-bit colours */
                fbpalette[i].r = *palette++ << 2;
                fbpalette[i].g = *palette++ << 2;
                fbpalette[i].b = *palette++ << 2;
            }
            break;
        case VIDEO_MODE_HIGH16:
        case VIDEO_MODE_HIGH16HALF:
            for(int i = 0; i < 16; ++i) {
                /* left-shift to convert the VGA-compatible
                 * 6-bit colours back to 8-bit colours */
                fbpalette[i].r = *palette++ << 2;
                fbpalette[i].g = *palette++ << 2;
                fbpalette[i].b = *palette++ << 2;
            }
            break;
        default:
            PANIC("Invalid video mode");
            break;
    }

    if(0 == SDL_SetPalette(framebuffer, SDL_LOGPAL | SDL_PHYSPAL, fbpalette, 0, 256)) {
        printf("%s\n", SDL_GetError());
        PANIC("Set palette failed");
    }
    
/*
    for(int i = 0; i < 256 * 3; i+= 3) {
        printf("[%d] (%d %d %d)\n", i, palette[i + 0], palette[i + 1], palette[i + 2]);
    } */
}
