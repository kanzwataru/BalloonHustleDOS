/*
 * Included directly into wb.c
*/
#include <stdlib.h>

struct RLEBlob {
    uint bg_len;
    uint fg_len;
};

static struct RLEBlob far *rle;
static Rect clouds[MAX_CLOUDS];
static Rect clouds_undo_rects[MAX_CLOUDS];
static bool clouds_pending = true;

#define CLOUDS_RENDER \
    for(i = 0; i < MAX_CLOUDS; ++i) { \
        clouds_render(clouds[i]); \
    }

static void clouds_rle(buffer_t *image)
{
    uint offset = 0, line = 0;
    struct RLEBlob far *p;

    rle = farcalloc((CLOUD_SPRITE_W * CLOUD_SPRITE_H) * 2, sizeof(struct RLEBlob));
    p = rle;
    assert(rle);

    while(offset < CLOUD_SPRITE_W * CLOUD_SPRITE_H) {
        while(line < CLOUD_SPRITE_W) {
            while(image[offset + line] == SKY_COL) {
                ++p->bg_len;
                ++line;

                if(line == CLOUD_SPRITE_W)
                    goto next;
            }
            while(image[offset + line] == CLOUD_COL) {
                ++p->fg_len;
                ++line;

                if(line == CLOUD_SPRITE_W)
                    goto next;
            }

            ++p;
        }

    next:
        offset += line;
        line = 0;
        ++p;
    }
}

static void clouds_render(Rect cloud)
{
    uint j = 0;
    byte lines = 0;
    buffer_t *p = rd.screen + (((cloud.y << 8) + (cloud.y << 6)) + cloud.x);
    struct RLEBlob far *r = rle;
    
    /* draw */
    
    while(lines < CLOUD_SPRITE_H) {
        j = 0;
        while(j < CLOUD_SPRITE_W) {
            j += r->bg_len;
            _fmemset(p + j, CLOUD_COL, r->fg_len);
            j += r->fg_len;

            ++r;
        }

        p += SCREEN_WIDTH;
        ++lines;
    }
}

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
        //clouds[i].x = 50 + rand() % 200;
        //clouds[i].y = rand() % 200;
        clouds[i].x = 0;
        clouds[i].y = 50;
        clouds[i].w = CLOUD_SPRITE_W;
        clouds[i].h = CLOUD_SPRITE_H;
    }
    
    //event_add(&clouds_update, NULL, CLOUDS_FRAME_SKIP);
    clouds_rle(cloud_image);
}
