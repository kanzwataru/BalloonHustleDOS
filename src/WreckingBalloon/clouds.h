/*
 * Included directly into wb.c
*/
#include <stdlib.h>

static Rect clouds[MAX_CLOUDS];
static Rect clouds_undo_rects[MAX_CLOUDS];
static bool clouds_pending = true;

#define CLOUDS_ERASE_DRAW \
    if(clouds_pending) { \
        for(i = 0; i < MAX_CLOUDS; ++i) { \
            draw_rect(rd.screen, clouds_undo_rects, SKY_COL); \
        } \
        for(i = 0; i < MAX_CLOUDS; ++i) { \
            draw_sprite_explicit(rd.screen, cloud_image, clouds[i], rd.screen_clipping); \
        } \
        clouds_pending = false; \
    } else {}

static void clouds_update(void *_)
{
    int i;
    for(i = 0; i < MAX_CLOUDS; ++i) {
        ++clouds[i].y;
    }

    clouds_pending = true;
}

static void clouds_init(void)
{
    int i;
    srand((unsigned int)clouds);

    for(i = 0; i < MAX_CLOUDS; ++i) {
        clouds[i].x = 50 + rand() % 200;
        clouds[i].y = rand() % 200;
        clouds[i].w = CLOUD_SPRITE_W;
        clouds[i].h = CLOUD_SPRITE_H;
    }
    
    event_add(&clouds_update, NULL, CLOUDS_FRAME_SKIP);
}
