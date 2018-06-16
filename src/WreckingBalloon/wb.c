#include "src/wrecki~1/wb.h"
#include "src/hustle~1/core.h"
#include "src/hustle~1/render.h"
#include "src/hustle~1/filesys.h"
#include "src/hustle~1/platform.h"

static RenderData rd;

static bool input(void)
{
    return true;
}

static void update(void)
{

}

static void render(void)
{

}

static void quit(void)
{

}

void wrecking_balloon_start(void) 
{
    CoreData cd;
    buffer_t *test_balloon;
    buffer_t *test_cactus;
    buffer_t *pal;

    cd.update_callback = &update;
    cd.render_callback = &render;
    cd.input_handler = &input;
    cd.exit_handler = &quit;
    cd.frame_skip = 0;

    test_balloon = create_image(48, 48);
    load_bmp_image(test_balloon, "RES\\BOON-A.BMP");

    test_cactus = create_image(48, 48);
    load_bmp_image(test_cactus, "RES\\CACTUS-A.BMP");

    pal = load_bmp_palette("RES\\BOON-A.BMP");

    init_renderer(&rd, 2, pal);
    rd.anim_frame_hold = 3;
    rd.flags = RENDER_DOUBLE_BUFFER;

    rd.sprites[0].vis.image = test_balloon;
    rd.sprites[0].rect.x = 128;
    rd.sprites[0].rect.y = 32;
    rd.sprites[0].rect.w = 48;
    rd.sprites[0].rect.h = 48;
    rd.sprites[0].flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_MASKED;

    rd.sprites[1].vis.image = test_cactus;
    rd.sprites[1].rect.x = 128;
    rd.sprites[1].rect.y = 96;
    rd.sprites[1].rect.w = 48;
    rd.sprites[1].rect.h = 48;
    rd.sprites[1].flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_MASKED;

    FILL_BUFFER(rd.screen, 1);
    FILL_BUFFER(rd.bg_layer, 1);
    refresh_sprites(&rd);
    engine_start(cd);
}
