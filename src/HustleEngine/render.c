#include "src/hustle~1/render.h"
#include "src/hustle~1/vga.h"
#include <malloc.h>
#include <dos.h>
#include <stdio.h>

static int anim_frame_count = 0;

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

static void blit(buffer_t *dest, const buffer_t *src, const Rect *rect)
{
    register int y = rect->h;
    register int offset = CALC_OFFSET(rect->x, rect->y);

    for(; y > 0; --y) {
        _fmemcpy(dest + offset, src + offset, rect->w);

        offset += SCREEN_WIDTH;
    }
}

static void blit_offset(buffer_t *dest, const buffer_t *src, const Rect *rect, int offset, int orig_w)
{
    register int y = rect->h;
    dest += CALC_OFFSET(rect->x, rect->y);
    src += offset;

    for(; y > 0; --y) {
        _fmemcpy(dest, src, rect->w);

        dest += SCREEN_WIDTH;
        src += orig_w;
    }
}

static void blit_offset_masked(buffer_t *dest, const buffer_t *src, const Rect *rect, int offset, int orig_w)
{
    register int x, y;

    dest += CALC_OFFSET(rect->x, rect->y);
    src += offset;

    for(y = 0; y < rect->h; ++y) {
        for(x = 0; x < rect->w; ++x) {
            if(src[x] != TRANSPARENT)
                *(dest + x) = *(src + x);
        }

        dest += SCREEN_WIDTH;
        src += orig_w;
    }
}

/*
static void blit_offset_masked(buffer_t *dest, const buffer_t *src, const Rect *rect, int offset, int orig_w)
{
    register int y = rect->h;
    register int x = rect->w;
    const int adj = SCREEN_WIDTH - rect->w;
    dest += CALC_OFFSET(rect->x, rect->y);
    src += offset;

    for(; y > 0; --y) {
        for(; x > 0; --x) {
            if(*src == TRANSPARENT) {
                ++dest;
                ++src;
            }
            else{
                *dest++ = *src++;
            }
        }
        dest += adj;
    }
}*/

void draw_rect(buffer_t *buf, const Rect *rect, byte colour)
{
    register int y = rect->h;
    register buffer_t *buf_o = buf + CALC_OFFSET(rect->x,rect->y);
    
    for(; y > 0; --y) {
        _fmemset(buf_o, colour, rect->w);
        buf_o += SCREEN_WIDTH;
    }
}


void reset_sprite(Sprite *sprite) {
    memset(sprite, 0, sizeof(Sprite));
}

void refresh_screen(RenderData *rd)
{
    if(++anim_frame_count > rd->anim_frame_hold) {
        anim_frame_count = 0;
    }
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
        blit(rd->screen, rd->bg_layer, dirt_rect);

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

        /* animation */
        if(sprite->anim && anim_frame_count == 0) {
            sprite->vis.image = sprite->anim->frames[sprite->current_frame];

            if(++sprite->current_frame > sprite->anim->frame_count - 1) {
                sprite->current_frame = 0;
            }
        }

        /* draw the sprite */
        if(sprite->flags & SPRITE_FILL) {
            draw_rect(rd->screen, r, sprite->vis.colour);
        }
        else if(sprite->flags & SPRITE_MASKED) {
            blit_offset_masked(rd->screen, sprite->vis.image, r, image_offset.x + (image_offset.y * r->w), sprite->rect.w);
        }
        else {
            blit_offset(rd->screen, sprite->vis.image, r, image_offset.x + (image_offset.y * r->w), sprite->rect.w);
        }
        
        *dirt_rect = *r;
    next:
        --sprite;
        --dirt_rect;
    } while(i--);
}

int init_renderer(RenderData *rd, int sprite_count, buffer_t *palette)
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

        if(palette)
            set_palette(palette);

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
