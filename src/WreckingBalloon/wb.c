#include "src/wrecki~1/wb.h"
#include "src/hustle~1/core.h"
#include "src/hustle~1/render.h"
#include "src/hustle~1/kb.h"
#include "src/hustle~1/filesys.h"
#include "src/hustle~1/platform.h"

#include "src/wrecki~1/consts.h"
#include "src/wrecki~1/cactoon.h"

static RenderData    rd;
static CactusBalloon player;

/******* SPRITE RESOURCES *********/
static buffer_t *player_balloon;
static buffer_t *player_cactus;
/**********************************/

static byte dir_input = 0;

static bool input(void)
{
    byte key = keyboard_read();
    switch(key) {
    case KEY_W:
        dir_input |= WB_UP;
        break;
    case KEY_S:
        dir_input |= WB_DOWN;
        break;
    case KEY_A:
        dir_input |= WB_LEFT;
        break;
    case KEY_D:
        dir_input |= WB_RIGHT;
        break;
    case RELEASED(KEY_W):
        dir_input &= ~WB_UP;
        break;
    case RELEASED(KEY_S):
        dir_input &= ~WB_DOWN;
        break;
    case RELEASED(KEY_A):
        dir_input &= ~WB_LEFT;
        break;
    case RELEASED(KEY_D):
        dir_input &= ~WB_RIGHT;
        break;
    case KEY_ESC:
        return false;
    }

    return true;
}

static void update(void)
{
    cactoon_move(&player, dir_input);
}

static void render(void)
{
    refresh_screen(&rd);
    refresh_sprites(&rd);
}

static void quit(void)
{
    quit_renderer(&rd);
}

static int num_of_sprites(void)
{
    return (MAX_CACTOONS * 2) + (MAX_BALLOONS * 2);
}

void wrecking_balloon_start(void) 
{
    CoreData cd;
    buffer_t *pal;

    cd.update_callback = &update;
    cd.render_callback = &render;
    cd.input_handler = &input;
    cd.exit_handler = &quit;
    cd.frame_skip = 0;

    player_balloon = create_image(48, 48);
    load_bmp_image(player_balloon, "RES\\BOON-A.BMP");

    player_cactus = create_image(48, 48);
    load_bmp_image(player_cactus, "RES\\CACTUS-A.BMP");

    pal = load_bmp_palette("RES\\BOON-A.BMP");

    init_renderer(&rd, num_of_sprites(), pal);
    destroy_image(&pal);

    rd.anim_frame_hold = 3;
    rd.flags = RENDER_DOUBLE_BUFFER;

    rd.sprites[0].vis.image = player_balloon;
    rd.sprites[0].rect.x = 128;
    rd.sprites[0].rect.y = 32;
    rd.sprites[0].rect.w = 48;
    rd.sprites[0].rect.h = 48;
    rd.sprites[0].flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_MASKED;

    rd.sprites[1].vis.image = player_cactus;
    rd.sprites[1].rect.x = 128;
    rd.sprites[1].rect.y = 96;
    rd.sprites[1].rect.w = 48;
    rd.sprites[1].rect.h = 48;
    rd.sprites[1].flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_MASKED;

    cactoon_init(&player, &rd.sprites[0], &rd.sprites[1]);

    FILL_BUFFER(rd.screen, 1);
    FILL_BUFFER(rd.bg_layer, 1);
    refresh_sprites(&rd);
    engine_start(cd);
}
