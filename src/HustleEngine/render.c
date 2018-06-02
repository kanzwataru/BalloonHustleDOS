#include "src/hustle~1/render.h"
#include <malloc.h>
#include <dos.h>

static int old_mode;  /* VGA mode we were in before switching to 13h */
static Rect *dirty_rects;

/*
 * Calculates the offset into the buffer (y * SCREEN_WIDTH + x)
 * Using bit shifting instead of mults for a bit of optimization
 *
 * (assumes mode 13h 320x200)
*/
#define CALC_OFFSET(x, y)             (((y) << 8) + ((y) << 6) + (x))

/*
 * Creates a buffer the size of the screen
*/
static buffer_t *make_framebuffer() {
    return farmalloc(SCREEN_SIZE);
}

buffer_t *create_image(uint w, uint h) {
    return farmalloc(w * h);
}

/*
 * Enters mode 13h
 *
 * (this code was lifted from: http://www3.telus.net/alexander_russell/course/chapter_1.htm)
*/
static void enter_m13h(void)
{
    union REGS in, out;

    // get old video mode
    in.h.ah = 0xf;
    int86(0x10, &in, &out);
    old_mode = out.h.al;

    // enter mode 13h
    in.h.ah = 0;
    in.h.al = 0x13;
    int86(0x10, &in, &out);
}

/*
 * Exits mode 13h
 *
 * (this code was lifted from: http://www3.telus.net/alexander_russell/course/chapter_1.htm)
*/
static void leave_m13h(void)
{
    union REGS in, out;

    in.h.ah = 0;
    in.h.al = old_mode;
    int86(0x10, &in, &out);
}

static void init_all_sprites(Sprite **sprites, const uint count) 
{
    if(count > 0) {
        *sprites = calloc(count, sizeof(Sprite));
        dirty_rects = calloc(count, sizeof(Rect));
    }
    else {
        *sprites = NULL;
    }
}

static void free_all_sprites(Sprite **sprites, uint *count)
{
    if(*sprites) {
        free(*sprites);
        *sprites = NULL;

        free(dirty_rects);
        dirty_rects = NULL;
    }

    *count = 0;
}

void reset_sprite(Sprite *sprite) {
    memset(sprite, 0, sizeof(Sprite));
}

void blit_rect(unsigned char far *dest, const unsigned char far *src, const Rect *rect)
{
    int y, offset;
    const int y_max = rect->y + rect->h;

    for(y = rect->y; y < y_max; ++y) {
        offset = CALC_OFFSET(rect->x, y);
        _fmemcpy(dest + offset, src + offset, rect->w);
    }
}

void refresh_sprites(RenderData *rd)
{
    //refresh_sprites_slow(rd);
    uint i;
    register int y;
    int offset, offset_sprite;
    int y_max;
    int x_clipped;

    const Sprite *sprite;
    for(i = 0; i < rd->sprite_count; ++i) {
        sprite = (i + rd->sprites);

        if(sprite->flags & SPRITE_REFRESH) {
            blit_rect(rd->screen, rd->bg_layer, &dirty_rects[i]);

            y_max = sprite->rect.y + sprite->rect.h;
            offset_sprite = 0;
            for(y = sprite->rect.y; y < y_max; ++y) {
                offset = CALC_OFFSET(sprite->rect.x, y);
                _fmemcpy(rd->screen + offset, sprite->image + offset_sprite, sprite->rect.w);
                offset_sprite += sprite->rect.h;
            }
        }
        dirty_rects[i] = sprite->rect;
    }
}

int init_renderer(RenderData *rd)
{
    rd->bg_layer = make_framebuffer();

    if(rd->bg_layer) {
        rd->screen = MK_FP(0xa000, 0);         /* this gets the screen framebuffer */
        enter_m13h();
        _fmemset(rd->bg_layer, 0, SCREEN_SIZE);
        init_all_sprites(&rd->sprites, rd->sprite_count);
        return 1;
    }
    else {
        leave_m13h();
        printf("out of mem\n");
        return 0;
    }
}

void quit_renderer(RenderData *rd)
{
    free_all_sprites(&rd->sprites, &rd->sprite_count);
    farfree(rd->bg_layer);
    leave_m13h();
}
