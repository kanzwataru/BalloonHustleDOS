#ifndef RENDER_H
#define RENDER_H
/*
 * HustleEngine Render routines
 *
*/

#include "src/hustle~1/platform.h"
#include "src/hustle~1/prim.h"

#define SCREEN_WIDTH 320   /* Mode 13h uses 320 x 200 res */
#define SCREEN_HEIGHT 200
#define SCREEN_SIZE 64000u /* Amount of pixels (320 x 200) */
#define MAX_SPRITE_SIZE 128
#define MAX_LINE_LENGTH 512
#define TRANSPARENT 0
#define DEFAULT_VGA_PALETTE 0
#define FILL_BUFFER(buf, col) _fmemset((buf), (col), (SCREEN_SIZE));

/*
 * Only using low nibble for engine
 * flags, rest is for game use
 * */
enum SPRITEFLAGS
{
    SPRITE_REFRESH    = 0x01, /* (0001) If the sprite should refresh */
    SPRITE_CLIP       = 0x02, /* (0010) If should be screen-clipped to not wrap back */
    SPRITE_FILL       = 0x04, /* (0100) If the sprite should be a colour and not an iamge */
    SPRITE_MASKED     = 0x08, /* (1000) If colour id 0 should be treated as transparent */
};

/*
 * Renderer Configuration flags
 * 
*/
enum RENDERFLAGS
{
    RENDER_DOUBLE_BUFFER = 0x01 /* (0000 0001) */
};

typedef void far* LineUndoList;

typedef struct {
    buffer_t    *frames;      /* sprite sheet, not owned */
    size_t      frame_size;
    byte        count;
    byte        skip;
} Animation;

typedef struct {
    union {
        buffer_t    *image;   /* not freed, reference only */
        byte        colour;
    } vis;
    Rect        rect;
    Rect        hitbox;
    Rect        *parent;      /* NULL by default, reference only */
    Animation   *anim;        /* NULL by default, reference only */
    byte        current_frame;
    byte        frame_skip_counter;
    byte        flags;
} Sprite;

typedef struct {
    Rect        screen_clipping;
    buffer_t    *screen;
    buffer_t    *bg_layer;
    Sprite      *sprites;
    uint        sprite_count;
    byte        flags;
} RenderData;

/**** Renderer functions ****/

buffer_t *create_image(uint w, uint h);
void destroy_image(buffer_t **image);
LineUndoList create_line_undo_list();
void destroy_line_undo_list(LineUndoList *lul);

int init_renderer(RenderData *rd, int sprite_count, buffer_t *palette);
int init_line_render(byte line_count);
void quit_renderer(RenderData *rd);

void start_frame(RenderData *rd);
void refresh_sprites(RenderData *rd);
void finish_frame(RenderData *rd);

void reset_sprite(Sprite *sprite);

/**** Drawing functions ****/

/*
 * Draw a filled rectangle with a specific colour
*/
void draw_rect(buffer_t *buf, const Rect *rect, byte colour);
void draw_dot(buffer_t *buf, Point p, byte colour);
void draw_line(buffer_t *buf, LineUndoList undo, const Point *p1, const Point *p2, const byte colour);
void erase_line(buffer_t *buf, LineUndoList undo);

#endif /* RENDER_H */