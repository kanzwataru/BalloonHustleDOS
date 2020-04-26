#include "hustle.h"
#include "platform/bootstrap.h"
#include "game.h"

struct GameData *g;

static void place_cloud(Rect *cloud)
{
	cloud->x = RANDRANGE(CLOUD_SIDE_MIN, CLOUD_SIDE_MAX);
	cloud->y = RANDRANGE(CLOUD_HEIGHT_MIN, CLOUD_HEIGHT_MAX);
	cloud->w = asset_get(CLOUD, Texture, g->pak)->width;
	cloud->h = asset_get(CLOUD, Texture, g->pak)->height;
}

static void scroll_clouds(void)
{
	for(int i = 0; i < CLOUD_MAX; ++i) {
		g->clouds[i].y -= CLOUD_SPEED;
		if(g->clouds[i].y < -100) {
			place_cloud(&g->clouds[i]);
		}
	}
}

static void create_balloon_cactus(entity_id id, entity_id cactus_id, bool is_player)
{
    /* anim table pointers */
    const struct BalloonAnimTable *balloon_anim = is_player ? &s_player_balloon_anim : &s_enemy_balloon_anim;
    const struct CactusAnimTable  *cactus_anim  = is_player ? &s_player_cactus_anim  : &s_enemy_cactus_anim;

    /* balloon */
    sprite_set_to(&g->sprites[id], asset_make_handle(balloon_anim->idle_anim, g->pak));
    g->sprites[id].rect.x = 320 / 2 - (g->sprites[id].rect.w / 2);
    g->sprites[id].rect.y = 36;

    g->transforms[id].enabled = true;
    g->transforms[id].pos.x = g->sprites[id].rect.x;
    g->transforms[id].pos.y = g->sprites[id].rect.y;

    g->balloons[id].enabled = true;
    g->balloons[id].constrain_to_screen = true;
    g->balloons[id].state = BALLOON_STATE_IDLE;
    g->balloons[id].anim_table = balloon_anim;

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
    const Rect br = {
        16, 8, 16, 22
    };
    g->colliders[id].rect = br;

    /* cactus */
    sprite_set_to(&g->sprites[cactus_id], asset_make_handle(cactus_anim->idle_anim, g->pak));
    g->sprites[cactus_id].rect.w = asset_from_handle_of(g->sprites[cactus_id].spritesheet, Spritesheet)->width;
    g->sprites[cactus_id].rect.h = asset_from_handle_of(g->sprites[cactus_id].spritesheet, Spritesheet)->height;

    g->transforms[cactus_id].enabled = true;

    g->colliders[cactus_id].enabled = true;
    g->colliders[cactus_id].type = COLL_CACTUS;
    const Rect cr = {
        12, 10, 24, 20
    };
    g->colliders[cactus_id].rect = cr;

    g->cactuses[cactus_id].enabled = true;
    g->cactuses[cactus_id].anim_table = cactus_anim;
    
    /* ai */
    if(!is_player) {
        g->ai[id].enabled = true;
        g->ai[id].state = AI_WAITING;
        g->ai[id].timer = 3 * 60;
        g->balloons[id].constrain_to_screen = false;
        g->transforms[id].pos.x = RANDRANGE(400, 700) * (RANDRANGE(0, 1) ? -1 : 1);
        g->transforms[id].pos.x = RANDRANGE(300, 700) * (RANDRANGE(0, 1) ? -1 : 1);
    }
}

void init(void)
{
    struct PaletteAsset *pal;

    pal = asset_get(GAMEPAL, Palette, g->pak);
    assert(pal->col_count <= sizeof(g->palette));
    memcpy(g->palette, pal, pal->col_count);
    renderer_set_palette(g->palette, 0, sizeof(g->palette));

    create_balloon_cactus(0, 1, true);
    create_balloon_cactus(2, 3, false);

    rope_init(0, 4);

    for(int i = 0; i < CLOUD_MAX; ++i) {
		 place_cloud(&g->clouds[i]);
    }
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
	if(g->game_state == GAME_STATE_BEGIN) {
		struct PaletteAsset *pal = asset_get(GAMEPAL, Palette, g->pak);
		g->fade_percent += 0.02f;
		if(g->fade_percent >= 1.0f) {
			g->fade_percent = 1.0f;
			g->game_state = GAME_STATE_PLAYING;
		}

		for(int i = 0; i < sizeof(g->palette); ++i) {
			g->palette[i] = LERP(0, pal->data[i], g->fade_percent);
			renderer_set_palette(g->palette, 0, sizeof(g->palette));
		}
	}

	ai_update(0, ENTITY_MAX);
    balloon_update(0, ENTITY_MAX);
    transform_update(0, ENTITY_MAX);
    rope_update(0, ENTITY_MAX);
    collider_update(0, ENTITY_MAX);
    sprite_update(g->sprites, ENTITY_MAX);

	 scroll_clouds();
}

void render(void)
{
    renderer_clear(1);

    for(int i = 0; i < CLOUD_MAX; ++i) {
     	renderer_draw_texture(asset_get(CLOUD, Texture, g->pak), g->clouds[i]);
     }

    rope_draw(0, ENTITY_MAX);
    sprite_draw(g->sprites, ENTITY_MAX);

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
