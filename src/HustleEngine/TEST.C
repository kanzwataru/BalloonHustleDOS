#include <stdio.h>
#include "src/hustle~1/render.h"
#include "src/hustle~1/core.h"

#define SPRITE_COUNT 5

RenderData rd;
byte col = 0;

void update(void) {
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

    rd.sprites[0].image = create_image(32,32);

    for(i = 0; i < 32 * 32; ++i) {
        rd.sprites[0].image[i] = i;
    }
    
    rd.sprites[0].rect.w = 32;
    rd.sprites[0].rect.h = 32;
    rd.sprites[0].rect.x = 16;
    rd.sprites[0].rect.y = 64;
    rd.sprites[0].flags = SPRITE_REFRESH | SPRITE_CLIP;

    rd.sprites[1].image = rd.sprites[0].image;
    rd.sprites[1].rect.w = 32;
    rd.sprites[1].rect.h = 32;
    rd.sprites[1].rect.x = 128;
    rd.sprites[1].rect.y = 128;
    rd.sprites[1].flags = SPRITE_REFRESH | SPRITE_CLIP;

    rd.sprites[2].image = rd.sprites[0].image;
    rd.sprites[2].rect.w = 32;
    rd.sprites[2].rect.h = 32;
    rd.sprites[2].rect.x = 256;
    rd.sprites[2].rect.y = 128;
    rd.sprites[2].flags = SPRITE_REFRESH | SPRITE_CLIP;

    FILL_BUFFER(rd.screen, 3);
    FILL_BUFFER(rd.bg_layer, 3);

    engine_start(cd);
    return 1;
}
