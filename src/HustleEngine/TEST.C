#include <stdio.h>
#include <stdlib.h>
#include "src/hustle~1/render.h"
#include "src/hustle~1/core.h"
#include "src/hustle~1/filesys.h"
#include "src/hustle~1/platform.h"

#define SPRITE_COUNT   8
#define BG_BRICK_SIZE 10
#define BG_BRICK_COL  4

Point bounce_dirs[SPRITE_COUNT];
RenderData rd;
byte col = 0;

Animation test_anim;

void add_bricks(void)
{
    int col = 0;
    Rect r = {0,0,BG_BRICK_SIZE, BG_BRICK_SIZE};
    
    while(r.y < (SCREEN_HEIGHT - BG_BRICK_SIZE)) {
        while(r.x < (SCREEN_WIDTH - BG_BRICK_SIZE)) {
            draw_rect(rd.screen, &r, ++col);
            r.x += BG_BRICK_SIZE * 2;
        }
        r.x = 0;
        r.y += BG_BRICK_SIZE * 2;
    }
}

void animation_frames_init(void)
{
    int i;
    init_animation(&test_anim, 48, 32, 32);

    for(i = 0; i < test_anim.frame_count; ++i) {
        FILL_BUFFER(test_anim.frames[i], 18 + i);
    }
}

void add_border(void)
{
    Rect btm = {0, 170, SCREEN_WIDTH, SCREEN_HEIGHT - 170};
    rd.screen_clipping.h = 170;
    draw_rect(rd.screen, &btm, 6);
}

void bouncing_sprites_init(void)
{
    Rect rect = {0,0,16,16};
    int i;
    for(i = 3; i < SPRITE_COUNT - 1; ++i) {
        rect.x = rand() % 320;
        rect.y = rand() % 200;
        rd.sprites[i].rect = rect;
        rd.sprites[i].vis.colour = rand() % 255;
        rd.sprites[i].flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_FILL;
        bounce_dirs[i].x = -1;
        bounce_dirs[i].y = 1;
    }
}

void bouncing_sprites_update(void)
{
    int i;
    for(i = 3; i < SPRITE_COUNT - 1; ++i) {
        rd.sprites[i].rect.x += bounce_dirs[i].x;
        rd.sprites[i].rect.y += bounce_dirs[i].y;

        if(rd.sprites[i].rect.x >= SCREEN_WIDTH)
            bounce_dirs[i].x = -1;
        else if(rd.sprites[i].rect.x < 0)
            bounce_dirs[i].x = 1;

        if(rd.sprites[i].rect.y >= SCREEN_HEIGHT)
            bounce_dirs[i].y = -1;
        else if(rd.sprites[i].rect.y < 0)
            bounce_dirs[i].y = 1;
    }
}

void update(void) {
    bouncing_sprites_update();
    rd.sprites[1].rect.x += 1;
    if(rd.sprites[1].rect.x > SCREEN_WIDTH + 100)
        rd.sprites[1].rect.x = -100;

    rd.sprites[SPRITE_COUNT - 1].rect.y += 1;
    if(rd.sprites[SPRITE_COUNT - 1].rect.y > SCREEN_HEIGHT + 32)
        rd.sprites[SPRITE_COUNT - 1].rect.y = -32;
}

void render(void) {
    /*FILL_BUFFER(rd.screen, col++);
    */
    refresh_screen(&rd);
    refresh_sprites(&rd);
}

bool input(void) {
    return true;
}

int main(int argc, char **argv)
{
    uint i;
    CoreData cd;
    buffer_t *balloon_img;
    buffer_t *pal;

    cd.update_callback = &update;
    cd.render_callback = &render;
    cd.input_handler = &input;
    cd.frame_skip = 0;

    balloon_img = create_image(32, 32);
    load_bmp_image(balloon_img, "RES\\BALLOON.BMP");
    pal = load_bmp_palette("RES\\BALLOON.BMP");

    rd.anim_frame_hold = 3;
    rd.flags |= RENDER_DOUBLE_BUFFER;

    if(!init_renderer(&rd, SPRITE_COUNT, pal))
        return 0;


    //rd.sprites[0].anim = &test_anim;
    rd.sprites[0].vis.colour = 4;
    rd.sprites[0].rect.w = 32;
    rd.sprites[0].rect.h = 32;
    rd.sprites[0].rect.x = 16;
    rd.sprites[0].rect.y = 64;
    rd.sprites[0].flags = SPRITE_REFRESH | SPRITE_FILL;

    rd.sprites[1].vis.image = create_image(32,32);

    for(i = 0; i < 32 * 32; ++i) {
        rd.sprites[1].vis.image[i] = i;
    }

    rd.sprites[1].vis.image = rd.sprites[1].vis.image;
    rd.sprites[1].rect.w = 32;
    rd.sprites[1].rect.h = 32;
    rd.sprites[1].rect.x = 128;
    rd.sprites[1].rect.y = 128;
    rd.sprites[1].flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_MASKED;

    rd.sprites[2].vis.colour = 4;
    rd.sprites[2].rect.w = 8;
    rd.sprites[2].rect.h = 16;
    rd.sprites[2].rect.x = 24;
    rd.sprites[2].rect.y = -8;
    rd.sprites[2].flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_FILL;
    rd.sprites[2].parent = &rd.sprites[1].rect;

    rd.sprites[SPRITE_COUNT - 1].vis.image = balloon_img;
    rd.sprites[SPRITE_COUNT - 1].rect.w = 32;
    rd.sprites[SPRITE_COUNT - 1].rect.h = 32;
    rd.sprites[SPRITE_COUNT - 1].rect.x = 256;
    rd.sprites[SPRITE_COUNT - 1].rect.y = 128;
    rd.sprites[SPRITE_COUNT - 1].flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_MASKED;

    //animation_frames_init();
    bouncing_sprites_init();
    FILL_BUFFER(rd.screen, 3);
    FILL_BUFFER(rd.bg_layer, 3);
    add_bricks();
    add_border();

    refresh_sprites(&rd);
    engine_start(cd);
    return 1;
}
