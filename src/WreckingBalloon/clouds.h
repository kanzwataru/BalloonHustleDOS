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

static void clouds_render(void)
{
    uint j;
    byte i, lines;
    buffer_t *p;
    struct RLEBlob far *r;

    /* erase */
    for(i = 0; i < MAX_CLOUDS; ++i) {
        draw_rect(rd.screen, &clouds[i], SKY_COL);
    }

    /* draw */
    for(i = 0; i < MAX_CLOUDS; ++i) {
        lines = 0;
        r = rle;
        p = rd.screen + (((clouds[i].y << 8) + (clouds[i].y << 6)) + clouds[i].x);

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
}

static void clouds_update(void *_)
{
    int i;
    for(i = 0; i < MAX_CLOUDS; ++i) {
        clouds_undo_rects[i] = clouds[i];
        --clouds[i].x;
    }
    clouds_pending = true;
    //event_add(&clouds_update, NULL, CLOUDS_FRAME_SKIP);
}

static void clouds_init(void)
{
    int i;
    srand((unsigned int)clouds);

    for(i = 0; i < MAX_CLOUDS; ++i) {
        clouds[i].x = (rand() % (200 - CLOUD_SPRITE_W)) + 50;
        clouds[i].y = rand() % (200 - CLOUD_SPRITE_H);
        clouds[i].w = CLOUD_SPRITE_W;
        clouds[i].h = CLOUD_SPRITE_H;

        clouds_undo_rects[i] = clouds[i];
    }
    
   //event_add(&clouds_update, NULL, CLOUDS_FRAME_SKIP);
    clouds_rle(cloud_image);
}
