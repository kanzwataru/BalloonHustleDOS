#include <stdio.h>
#include "src/hustle~1/render.h"
#include "src/hustle~1/core.h"

#define SPRITE_COUNT 10

Point bounce_dirs[SPRITE_COUNT];
RenderData rd;
byte col = 0;

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
    for(i = 3; i < SPRITE_COUNT; ++i) {
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
    for(i = 3; i < SPRITE_COUNT; ++i) {
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
    rd.sprites[0].rect.h -= 1;
    if(rd.sprites[0].rect.h == 1)
        rd.sprites[0].rect.h = 32;

    rd.sprites[1].rect.x += 1;
    if(rd.sprites[1].rect.x > SCREEN_WIDTH + 100)
        rd.sprites[1].rect.x = -100;

    rd.sprites[2].rect.y += 1;
    if(rd.sprites[2].rect.y > SCREEN_HEIGHT + 32)
        rd.sprites[2].rect.y = -32;
}

void render(void) {
    /*FILL_BUFFER(rd.screen, col++);
    */
    refresh_sprites(&rd);
}

bool input(void) {
    return true;
}

int main(int argc, char **argv)
{
    uint i;
    CoreData cd;

    cd.update_callback = &update;
    cd.render_callback = &render;
    cd.input_handler = &input;

    if(!init_renderer(&rd, SPRITE_COUNT))
        return 0;

    rd.sprites[0].vis.image = create_image(32,32);

    for(i = 0; i < 32 * 32; ++i) {
        rd.sprites[0].vis.image[i] = i;
    }
    
    rd.sprites[0].rect.w = 32;
    rd.sprites[0].rect.h = 32;
    rd.sprites[0].rect.x = 16;
    rd.sprites[0].rect.y = 64;
    rd.sprites[0].flags = SPRITE_REFRESH | SPRITE_CLIP;

    rd.sprites[1].vis.image = rd.sprites[0].vis.image;
    rd.sprites[1].rect.w = 32;
    rd.sprites[1].rect.h = 32;
    rd.sprites[1].rect.x = 128;
    rd.sprites[1].rect.y = 128;
    rd.sprites[1].flags = SPRITE_REFRESH | SPRITE_CLIP;

    rd.sprites[2].vis.image = rd.sprites[0].vis.image;
    rd.sprites[2].rect.w = 32;
    rd.sprites[2].rect.h = 32;
    rd.sprites[2].rect.x = 256;
    rd.sprites[2].rect.y = 128;
    rd.sprites[2].flags = SPRITE_REFRESH | SPRITE_CLIP;

    bouncing_sprites_init();
    FILL_BUFFER(rd.screen, 3);
    FILL_BUFFER(rd.bg_layer, 3);
    add_border();

    engine_start(cd);
    return 1;
}
