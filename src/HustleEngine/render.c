#include "src/hustle~1/render.h"
#include <malloc.h>
#include <dos.h>
#include <stdio.h>

static int old_mode;  /* VGA mode we were in before switching to 13h */
static Rect *dirty_rects;
static const Rect EMPTY_RECT = {0,0,0,0};

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

static bool clip_rect(Rect *clipped, Point *offset, const Rect *orig, const Rect *clip)
{
    register int o_xmax = orig->x + orig->w;
    register int o_ymax = orig->y + orig->h;
    register int c_xmax = clip->x + clip->w;
    register int c_ymax = clip->y + clip->h;

    if(orig->x > c_xmax || orig->y > c_ymax) /* completely offscreen */
        return false;

    /* Horizontal clip */
    if(orig->x < clip->x) { /* left */
        if(o_xmax < clip->x) return false; /* completely offscreen */
        
        offset->x = (clip->x - orig->x);

        clipped->x = clip->x;
        clipped->w = orig->w - offset->x;
    }
    else if(o_xmax > c_xmax) {  /* right */
        offset->x = 0;

        clipped->x = orig->x;
        clipped->w = c_xmax - orig->x;
    }
    else {                  /* not clipped */
        offset->x = 0;

        clipped->x = orig->x;
        clipped->w = orig->w;
    }

    /* Vertical clip */
    if(orig->y < clip->y) {
        if(o_ymax < clip->y) return false; /* completely offscreen */

        offset->y = (clip->y - orig->y);

        clipped->y = clip->y;
        clipped->h = orig->h - offset->y;
    }
    else if(o_ymax > c_ymax) {
        offset->y = 0;

        clipped->y = orig->y;
        clipped->h = c_ymax - orig->y;
    }
    else {
        offset->y = 0;

        clipped->y = orig->y;
        clipped->h = orig->h;
    }

    return true;
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

void reset_sprite(Sprite *sprite) {
    memset(sprite, 0, sizeof(Sprite));
}

void refresh_sprites(RenderData *rd)
{
    register int y;
    register int offset;
    int y_max;
    Rect clipped;
    Point image_offset;
    Rect *r;

    uint i = rd->sprite_count;
    Sprite *sprite = rd->sprites + i;
    Rect *dirt_rect = dirty_rects + i;
    do {
        /* skip sprites that aren't active */
        if(!(sprite->flags & SPRITE_REFRESH)) {
            goto next;
        }

        /* copy BG over dirty rect */
        blit_rect(rd->screen, rd->bg_layer, dirt_rect);

        /* check if we need to clip the sprite
         * or let it overflow */
        if(sprite->flags & SPRITE_CLIP) {
            r = &clipped;
            if(!clip_rect(&clipped, &image_offset, &sprite->rect, &rd->screen_clipping)) {
                *dirt_rect = EMPTY_RECT;
                goto next;
            }
        }
        else {
            image_offset.x = 0;
            image_offset.y = 0;
            r = &sprite->rect;
        }

        /* draw the sprite */
        if(sprite->flags & SPRITE_FILL) {
            draw_rect(rd->screen, r, sprite->vis.colour);
        }
        else {
            y_max = r->y + r->h;
            image_offset.x += image_offset.y * r->w;
            for(y = r->y; y < y_max; ++y) {
                offset = CALC_OFFSET(r->x, y);
                _fmemcpy(
                    rd->screen + offset, 
                    sprite->vis.image + image_offset.x,
                    r->w);

                image_offset.x += sprite->rect.w;
            }
        }
        
        *dirt_rect = *r;
    next:
        --sprite;
        --dirt_rect;
    } while(i--);
}

int init_renderer(RenderData *rd, int sprite_count)
{
    rd->bg_layer = make_framebuffer();
    rd->sprite_count = sprite_count;

    if(rd->bg_layer) {
        rd->screen = MK_FP(0xa000, 0);         /* this gets the screen framebuffer */
        enter_m13h();
        _fmemset(rd->bg_layer, 0, SCREEN_SIZE);
        init_all_sprites(&rd->sprites, sprite_count);

        rd->screen_clipping.x = 0;
        rd->screen_clipping.y = 0;
        rd->screen_clipping.w = SCREEN_WIDTH;
        rd->screen_clipping.h = SCREEN_HEIGHT;   

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

void draw_rect(buffer_t *buf, const Rect *rect, byte colour)
{
    register int y;
    register int x = rect->x;
    register int y_max = rect->y + rect->h;
    register int w = rect->w;

    for(y = rect->y; y < y_max; ++y) {
        _fmemset(buf + (CALC_OFFSET(x, y)), colour, w);
    }
}
