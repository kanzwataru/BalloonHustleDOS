#include "hustle.h"
#include "platform/bootstrap.h"
#include "components.h"
#include "game.h"

struct GameData *g;

static void create_player_balloon(entity_id id, entity_id cactus_id)
{
    /* balloon */
    sprite_set_to(&g->sprites[id], asset_handle_to(BALLOON_IDLE, Spritesheet, g->pak));
    g->sprites[id].rect.x = 320 / 2 - (g->sprites[id].rect.w / 2);
    g->sprites[id].rect.y = 36;

    g->transforms[id].enabled = true;
    g->transforms[id].pos.x = g->sprites[id].rect.x;
    g->transforms[id].pos.y = g->sprites[id].rect.y;

    g->balloons[id].enabled = true;
    g->balloons[id].constrain_to_screen = true;
    g->balloons[id].state = BALLOON_STATE_IDLE;

    g->ropes[id].enabled = true;
    g->ropes[id].color = 8;
    g->ropes[id].start_transform = id;
    g->ropes[id].end_transform = cactus_id;
    g->ropes[id].offset.x = 25;
    g->ropes[id].offset.y = 39;
    g->ropes[id].end_offset.x = -24;
    g->ropes[id].end_offset.y = -7;

    g->colliders[id].enabled = true;
    g->colliders[id].type = COLL_BALLOON;
    g->colliders[id].rect = (Rect) {
        16, 8, 16, 22
    };

    /* cactus */
    g->sprites[cactus_id].spritesheet = asset_handle_to(CAC_IDLE, Spritesheet, g->pak);
    g->sprites[cactus_id].rect.w = asset_from_handle_of(g->sprites[cactus_id].spritesheet, Spritesheet)->width;
    g->sprites[cactus_id].rect.h = asset_from_handle_of(g->sprites[cactus_id].spritesheet, Spritesheet)->height;

    g->transforms[cactus_id].enabled = true;

    g->colliders[cactus_id].enabled = true;
    g->colliders[cactus_id].type = COLL_CACTUS;    
    g->colliders[cactus_id].rect = (Rect) {
        12, 10, 24, 20
    };

    g->cactuses[cactus_id].enabled = true;
}

void init(void)
{
    struct PaletteAsset *pal;

    pal = asset_get(GAMEPAL, Palette, g->pak);
    renderer_set_palette(pal->data, 0, pal->col_count);

    create_player_balloon(0, 1);
    rope_init(0, 2);
}

void input(void)
{
    struct BalloonComp *balloon = &g->balloons[0];
    balloon->dir.x = 0;
    balloon->dir.y = 0;
    if(keyboard_keys[KEY_RIGHT]) {
        balloon->dir.x = 1;
    }
    else if(keyboard_keys[KEY_LEFT]) {
        balloon->dir.x = -1;
    }
    if(keyboard_keys[KEY_UP]) {
        balloon->dir.y = -1;
    }
    else if(keyboard_keys[KEY_DOWN]) {
        balloon->dir.y = 1;
    }
}

void update(void)
{
    balloon_update(0, 2);
    transform_update(0, 2);
    rope_update(0, 2);
    collider_update(0, 2);
    sprite_update(g->sprites, 2);
}

void render(void)
{
    renderer_clear(2);
    rope_draw(0, 2);
    sprite_draw(g->sprites, 2);

//#define DEBUG_DRAW
#ifdef DEBUG_DRAW
    for(int i = 0; i < ENTITY_MAX; ++i) {
        if(g->colliders[i].enabled && g->transforms[i].enabled) {
            Rect r = g->colliders[i].rect;
            r.x += g->transforms[i].pos.x;
            r.y += g->transforms[i].pos.y;

            renderer_draw_rect(3, r);
        }
    }
#endif

    renderer_flip();
}
void quit(void) {}

void HANDSHAKE_FUNCTION_NAME(struct Game *game, void *memory_chunk) 
{
    g = (struct GameData *)memory_chunk;
    g->game = game;

    game->init = init;
    game->input = input;
    game->update = update;
    game->render = render;
    game->quit = quit;

    asset_load_pak(g->pak, "main.dat");
}

