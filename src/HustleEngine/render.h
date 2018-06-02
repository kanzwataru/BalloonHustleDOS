#ifndef RENDER_H
#define RENDER_H
/*
 * HustleEngine Render routines
 *
*/

#include "src/hustle~1/types.h"

#define SCREEN_WIDTH 320   /* Mode 13h uses 320 x 200 res */
#define SCREEN_HEIGHT 200
#define SCREEN_SIZE 64000u /* Amount of pixels (320 x 200) */
#define FILL_BUFFER(buf, col) _fmemset((buf), (col), (SCREEN_SIZE));

/*
 * Only using low nibble for engine
 * flags, rest is for game use
 * */
enum SPRITEFLAGS
{
    SPRITE_REFRESH    = 0x01, /* (0001) If the sprite should refresh */
    SPRITE_CLIP       = 0x02  /* (0010) If should be screen-clipped to not wrap back */
};

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct {
    buffer_t    **frames;      /* owned */
    byte        frame_count;
} Animation;

typedef struct {
    buffer_t    *image;       /* not freed, reference only */
    Rect        rect;
    Rect        hitbox;
    Rect        *parent;      /* NULL by default, reference only */
    Animation   *anim;        /* NULL by default, reference only */
    byte        anim_frame;
    byte        flags;
} Sprite;

typedef struct {
    buffer_t *screen;
    buffer_t *bg_layer;
    Sprite *sprites;
    uint sprite_count;
} RenderData;

buffer_t *create_image(uint w, uint h);
int init_renderer(RenderData *rd);
void quit_renderer(RenderData *rd);

void refresh_sprites(RenderData *rd);

void reset_sprite(Sprite *sprite);

#endif /* RENDER_H */