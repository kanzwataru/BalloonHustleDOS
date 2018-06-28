#include "src/wrecki~1/wb.h"
#include "src/hustle~1/core.h"
#include "src/hustle~1/render.h"
#include "src/hustle~1/kb.h"
#include "src/hustle~1/filesys.h"
#include "src/hustle~1/platform.h"

#include "src/wrecki~1/consts.h"
#include "src/wrecki~1/cactoon.h"
#include "src/wrecki~1/resource.h"

#include <dos.h>
#include <stdio.h>

static RenderData    rd;
static CactusBalloon player;

static LineUndoList  cactoon_strings[MAX_CACTOONS];
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
    cactoon_update(&player, dir_input);
}

static void debug_render_hitboxes(void)
{
    Rect hbox_a, hbox_b;

    hbox_a.x = player.balloon->rect.x + player.balloon->hitbox.x;
    hbox_a.y = player.balloon->rect.y + player.balloon->hitbox.y;
    hbox_a.w = player.balloon->hitbox.w;
    hbox_a.h = player.balloon->hitbox.h;

    hbox_b.x = player.cactus->rect.x + player.cactus->hitbox.x;
    hbox_b.y = player.cactus->rect.y + player.cactus->hitbox.y;
    hbox_b.w = player.cactus->hitbox.w;
    hbox_b.h = player.cactus->hitbox.h;

    draw_rect(rd.screen, &hbox_a, 7);
    draw_rect(rd.screen, &hbox_b, 8);
}

static void render(void)
{
    int i;
    Point a, b;

    start_frame(&rd);
    for(i = 0; i < MAX_CACTOONS; ++i)
        erase_line(rd.screen, cactoon_strings[0]);
    if(!(player.flags & CT_DEAD)) {
        a.x = player.balloon->rect.x + CACTOON_SPRITE_HALF;
        a.y = player.balloon->rect.y + BALLOON_STRING_OFFSET;
        b.x = player.cactus->rect.x + CACTOON_SPRITE_HALF;
        b.y = player.cactus->rect.y + CACTUS_STRING_OFFSET;
        draw_line(rd.screen, cactoon_strings[0], &a, &b, STRING_COL);
    }
    refresh_sprites(&rd);

    //debug_render_hitboxes();

    finish_frame(&rd);
}

static void quit(void)
{
    int i;
    for(i = 0; i < MAX_CACTOONS; ++i) {
        destroy_line_undo_list(&cactoon_strings[i]);
    }

    free_all_resources();
    quit_renderer(&rd);
}

static int num_of_sprites(void)
{
    return (MAX_CACTOONS * 2) + (MAX_BALLOONS * 2);
}

void wrecking_balloon_start(void) 
{
    int i;
    CoreData cd;
    buffer_t *pal;

    init_all_resources();

    cd.update_callback = &update;
    cd.render_callback = &render;
    cd.input_handler = &input;
    cd.exit_handler = &quit;
    cd.frame_skip = 0;

    pal = load_bmp_palette("RES\\CACP.BMP");

    init_renderer(&rd, num_of_sprites(), pal);
    destroy_image(&pal);

    for(i = 0; i < MAX_CACTOONS; ++i) {
        cactoon_strings[i] = create_line_undo_list();
    }

    rd.flags = RENDER_DOUBLE_BUFFER;

    cactoon_init(&player, &rd.sprites[0], &rd.sprites[1], 128, 32, 0);

    FILL_BUFFER(rd.screen, 1);
    FILL_BUFFER(rd.bg_layer, 1);
    refresh_sprites(&rd);
    engine_start(cd);
}
