#include "src/RLETEST.H"

#include "src/hustle~1/render.h"
#include "src/hustle~1/filesys.h"
#include "src/hustle~1/core.h"

#include "src/wrecki~1/consts.h"

static RenderData rd;
static RLEImageMono *cloud;
static Rect prev_cloud_rect;
static Rect cloud_rect;

#define CLOUD_SPRITE_W 128
#define CLOUD_SPRITE_H 48

static void load_stuff(void)
{
    buffer_t *cloud_raw = load_bmp_image("RES\\CLOUD.BMP");

    cloud = monochrome_buffer_to_rle(cloud_raw, CLOUD_SPRITE_W, CLOUD_SPRITE_H, CLOUD_COL, SKY_COL);

    destroy_image(&cloud_raw);    
}

static void update(void) 
{
    prev_cloud_rect = cloud_rect;

    cloud_rect.y += 1;
    if(cloud_rect.y + cloud_rect.h >= SCREEN_HEIGHT + (cloud_rect.h * 3))
        cloud_rect.y = -cloud_rect.h * 3;

    //printf("%d\n", cloud_rect.y);
}

static void render(void)
{
    Rect clipped;

    start_frame(&rd);

    draw_rect_clipped(rd.screen, &prev_cloud_rect, SKY_COL);
    //draw_rect(rd.screen, &cloud_rect, CLOUD_COL);
    //_fmemset(rd.screen, SKY_COL, SCREEN_SIZE);
    draw_mono_masked_rle_clipy(rd.screen, cloud, &cloud_rect, CLOUD_COL);
    //rd.screen[cloud_rect.y] = CLOUD_COL;
    finish_frame(&rd);

    //getch();
    //printf("%d\n", cloud_rect.y);
    //getch();
}

static bool input(void)
{   
    return !kbhit();
}

static void quit(void)
{
    quit_renderer(&rd);

    exit(1);
}

static void monorle_dump(RLEImageMono *rle, int width, int height)
{
    int pcount = 0;
    int print_count = 0;

    while(pcount < width * height) {
        printf("(%d %d)", rle->fglen, rle->bglen);
        if(++print_count == 4) {
            print_count = 0;
            printf("\n");
        }

        pcount += rle->fglen + rle->bglen;
        ++rle;
    }

    printf("\n Press any key to start...");
    getch();
}

void rletest_start(void)
{
    CoreData cd;
    buffer_t *pal;

    load_stuff();    
    monorle_dump(cloud, CLOUD_SPRITE_W, CLOUD_SPRITE_H);

    // init engine and rendererer
    cd.update_callback = &update;
    cd.render_callback = &render;
    cd.input_handler = &input;
    cd.exit_handler = &quit;
    cd.frame_skip = 0;

    pal = load_bmp_palette("RES\\CACP.BMP");

    rd.flags |= RENDER_DOUBLE_BUFFER;
    init_renderer(&rd, 2, pal);

    destroy_image(&pal);

    // clear screen
    _fmemset(rd.screen, SKY_COL, SCREEN_SIZE);
    
    // init clouds
    cloud_rect.x = 0;
    cloud_rect.y = 0;
    cloud_rect.w = CLOUD_SPRITE_W;
    cloud_rect.h = CLOUD_SPRITE_H;
    prev_cloud_rect = cloud_rect;

    engine_start(cd);
}
