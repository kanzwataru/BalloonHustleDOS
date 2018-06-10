#include "src/hustle~1/render.h"
#include "src/hustle~1/vga.h"
#include <malloc.h>
#include <dos.h>
#include <stdio.h>

struct SimpleSprite {
    buffer_t *image;
    Rect rect;
};

static int anim_frame_count = 0;

static struct SimpleSprite *dirty_tiles;
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
    int i;

    if(count > 0) {
        *sprites = calloc(count, sizeof(Sprite));
        dirty_tiles = calloc(count, sizeof(struct SimpleSprite));
        if(!dirty_tiles)
            while(1) printf("OUT OF MEM: dirty_tiles\n");
        for(i = 0; i < count; ++i) {
            dirty_tiles[i].image = farcalloc(MAX_SPRITE_SIZE * MAX_SPRITE_SIZE, sizeof(byte));
            if(!dirty_tiles[i].image)
                while(1) printf("OUT OF MEM: dirty_tiles (%d)\n", i);
        }
    }
    else {
        *sprites = NULL;
    }
}

static void free_all_sprites(Sprite **sprites, uint *count)
{
    int i;
    if(*sprites) {
        free(*sprites);
        *sprites = NULL;

        free(dirty_tiles);
        dirty_tiles = NULL;
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

static void tile_to_screen(buffer_t *dest, const buffer_t *src, const Rect *rect)
{
    register int y = rect->h;

    dest += CALC_OFFSET(rect->x, rect->y);

    for(; y > 0; --y) {
        _fmemcpy(dest, src, rect->w);

        dest += SCREEN_WIDTH;
        src += rect->w;
    }
}

static void screen_to_tile(buffer_t *dest, const buffer_t *src, const Rect *rect)
{
    register int y = rect->h;

    src += CALC_OFFSET(rect->x, rect->y);

    for(; y > 0; --y) {
        _fmemcpy(dest, src, rect->w);

        dest += rect->w;
        src += SCREEN_WIDTH;
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
    int i;

    if(++anim_frame_count > rd->anim_frame_hold) {
        anim_frame_count = 0;
    }

    for(i = rd->sprite_count - 1; i >= 0; --i) {
        tile_to_screen(rd->screen, dirty_tiles[i].image, &dirty_tiles[i].rect);
    }
}

void refresh_sprites(RenderData *rd)
{
    register int y;
    register int offset;
    static byte col;
    int y_max;
    Point image_offset;
    Rect r = EMPTY_RECT;

    size_t i;
    Sprite *sprite;
    struct SimpleSprite *d_tile;
    for(i = 0; i < rd->sprite_count; ++i) {
        sprite = rd->sprites + i;
        d_tile = dirty_tiles + i;

        /* skip sprites that aren't active */
        if(!(sprite->flags & SPRITE_REFRESH)) {
            d_tile->rect = EMPTY_RECT;
            continue;
        }

        /* check if we need to clip the sprite
         * or let it overflow */
        if(!clip_rect(&r, &image_offset, &sprite->rect, &rd->screen_clipping)) {
            d_tile->rect = EMPTY_RECT;
            continue;
        }

        /* animation */
        if(sprite->anim && anim_frame_count == 0) {
            sprite->vis.image = sprite->anim->frames[sprite->current_frame];

            if(++sprite->current_frame > sprite->anim->frame_count - 1) {
                sprite->current_frame = 0;
            }
        }

        screen_to_tile(d_tile->image, rd->screen, &r);
        d_tile->rect = r;

        /* draw the sprite */
        if(sprite->flags & SPRITE_FILL) {
            draw_rect(rd->screen, &r, sprite->vis.colour);
        }
        else if(sprite->flags & SPRITE_MASKED) {
            blit_offset_masked(rd->screen, sprite->vis.image, &r, image_offset.x + (image_offset.y * r.w), sprite->rect.w);
        }
        else {
            blit_offset(rd->screen, sprite->vis.image, &r, image_offset.x + (image_offset.y * r.w), sprite->rect.w);
        }
    };
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
