/*
 * Included directly into wb.c
*/
#include <stdlib.h>

struct RLEBlob {
    uint16 bg_len;
    uint16 fg_len;
};

static struct RLEBlob far *rle;
static Rect clouds[MAX_CLOUDS];
static Rect clouds_undo_rects[MAX_CLOUDS];
static bool clouds_pending = true;
static int  clouds_timer = CLOUDS_FRAME_SKIP;

#define CLOUDS_RENDER \
    if(clouds_pending) { \
        clouds_render(); \
    } else {}

#define CLOUDS_UPDATE \
    if(--clouds_timer == 0) {             \
        clouds_timer = CLOUDS_FRAME_SKIP; \
        clouds_update();                  \
    } else {}

static void clouds_rle(buffer_t *image)
{
    uint16 offset = 0, line = 0;
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
    uint16 j;
    byte i, lines, lineskip;
    buffer_t *p;
    struct RLEBlob far *r;

    for(i = 0; i < MAX_CLOUDS; ++i) {
        if(clouds_undo_rects[i].y + CLOUD_SPRITE_H <= 0 || clouds[i].y >= SCREEN_HEIGHT)
            continue;

        draw_rect_clipped(rd.screen, &clouds_undo_rects[i], SKY_COL);
    }

    for(i = 0; i < MAX_CLOUDS; ++i) {
        if(clouds[i].y + CLOUD_SPRITE_H <= 0 || clouds[i].y >= SCREEN_HEIGHT)
            continue;

        lines = (clouds[i].y + CLOUD_SPRITE_H) > SCREEN_HEIGHT ? SCREEN_HEIGHT - clouds[i].y : CLOUD_SPRITE_H;
        lineskip = clouds[i].y < 0 ? abs(clouds[i].y) : 0;

        /* draw */
        r = rle;
        p = rd.screen + (((clouds[i].y << 8) + (clouds[i].y << 6)) + clouds[i].x);

        while(lineskip != 0) {
            j = 0;
            while(j < CLOUD_SPRITE_W) {
                j += r->bg_len + r->fg_len;
                ++r;
            }
            --lineskip;
            --lines;
            p += SCREEN_WIDTH;
        }

        while(lines != 0) {
            j = 0;
            while(j < CLOUD_SPRITE_W) {
                j += r->bg_len;
                _fmemset(p + j, CLOUD_COL, r->fg_len);
                j += r->fg_len;

                ++r;
            }

            p += SCREEN_WIDTH;
            --lines;
        }
    }

    clouds_pending = false;
}

static void clouds_update(void)
{
    int i;
    for(i = 0; i < MAX_CLOUDS; ++i) {
        clouds_undo_rects[i] = clouds[i];
        clouds[i].y -= CLOUDS_SPEED;
        if(clouds[i].y <= -50) {
            clouds[i].x = (rand() % (200 - CLOUD_SPRITE_W)) + 50;
            clouds[i].y = 200 + (rand() % 50);
        };
    }
    clouds_pending = true;
}

static void clouds_init(void)
{
    int i;
    srand((unsigned int)clouds);

    for(i = 0; i < MAX_CLOUDS; ++i) {
        clouds[i].x = (rand() % (200 - CLOUD_SPRITE_W)) + 50;
        clouds[i].y = rand() % 300;
        clouds[i].w = CLOUD_SPRITE_W;
        clouds[i].h = CLOUD_SPRITE_H;

        clouds_undo_rects[i] = clouds[i];
    }
    
    clouds_rle(cloud_image);
}
