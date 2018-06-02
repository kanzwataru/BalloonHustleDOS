#include <stdio.h>
#include "src/hustle~1/render.h"
#include "src/hustle~1/core.h"

RenderData rd;
byte col = 0;

void update(void) {
    rd.sprites[0].rect.x += 1;
    if(rd.sprites[0].rect.x > SCREEN_WIDTH)
        rd.sprites[0].rect.x = 0;
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
    rd.sprite_count = 5;

    init_renderer(&rd);

    rd.sprites[0].image = create_image(32,32);

    for(i = 0; i < 32 * 32; ++i) {
        rd.sprites[0].image[i] = i;
    }
    
    rd.sprites[0].rect.w = 32;
    rd.sprites[0].rect.h = 32;
    rd.sprites[0].rect.x = 16;
    rd.sprites[0].rect.y = 64;
    rd.sprites[0].flags = SPRITE_REFRESH | SPRITE_CLIP;

    FILL_BUFFER(rd.screen, 3);
    FILL_BUFFER(rd.bg_layer, 3);

    engine_start(cd);
    return 1;
}
