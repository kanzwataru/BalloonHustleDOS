#include "hustle.h"
#include "platform/bootstrap.h"
#include "game.h"

//TODO: maybe these should be somewhere better?
#include <math.h>
#include <string.h>
#include <stdlib.h>

struct GameData *g;

static void place_cloud(Rect *cloud)
{
	cloud->x = RANDRANGE(CLOUD_SIDE_MIN, CLOUD_SIDE_MAX);
	cloud->y = RANDRANGE(CLOUD_HEIGHT_MIN, CLOUD_HEIGHT_MAX);
	cloud->w = g->pak.cloud.width;
	cloud->h = g->pak.cloud.height;
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

#define CLEAR(x) memset(&(x), 0, sizeof((x)))

void create_bullet(entity_id id, const Point *pos, const FLPoint *dir)
{
	entity_reserve(g->entity_roster, id);

	CLEAR(g->sprites[id]);
	sprite_set_to(&g->sprites[id], asset_handle_to(bullet, &g->pak, struct PakMain));

	CLEAR(g->transforms[id]);
	g->transforms[id].enabled = true;
	g->transforms[id].pos.x = pos->x;
	g->transforms[id].pos.y = pos->y;

	CLEAR(g->colliders[id]);
	g->colliders[id].enabled = true;
	g->colliders[id].type = COLL_BULLET;
	const Rect os = {
		1, 2, 6, 4
	};
	g->colliders[id].rect = os;

	CLEAR(g->motors[id]);
	g->motors[id].speed = BULLET_SPEED;
	g->motors[id].dir = *dir;

	//CLEAR(g->kills[id]);
	g->kills[id].enabled = true;
}

void create_balloon_cactus(entity_id id, entity_id cactus_id, bool is_player, bool reserve)
{
    /* anim table pointers */
    const struct BalloonAnimTable *balloon_anim = is_player ? &s_player_balloon_anim : &s_enemy_balloon_anim;
    const struct CactusAnimTable  *cactus_anim  = is_player ? &s_player_cactus_anim  : &s_enemy_cactus_anim;

    /* balloon */
	if(reserve)
		entity_reserve(g->entity_roster, id);

    CLEAR(g->sprites[id]);
    sprite_set_to(&g->sprites[id], asset_make_handle(balloon_anim->idle_anim, &g->pak));
    g->sprites[id].rect.x = 320 / 2 - (g->sprites[id].rect.w / 2);
    g->sprites[id].rect.y = 36;

    CLEAR(g->transforms[id]);
    g->transforms[id].enabled = true;
    g->transforms[id].pos.x = g->sprites[id].rect.x;
    g->transforms[id].pos.y = g->sprites[id].rect.y;

    CLEAR(g->balloons[id]);
    g->balloons[id].enabled = true;
    g->balloons[id].constrain_to_screen = true;
    g->balloons[id].state = BALLOON_STATE_IDLE;
    g->balloons[id].anim_table = balloon_anim;

    CLEAR(g->ropes[id]);
    g->ropes[id].enabled = true;
    g->ropes[id].color = 8;
    g->ropes[id].start_transform = id;
    g->ropes[id].end_transform = cactus_id;
    g->ropes[id].offset.x = 25;
    g->ropes[id].offset.y = 39;
    g->ropes[id].end_offset.x = -24;
    g->ropes[id].end_offset.y = -7;

	CLEAR(g->colliders[id]);
    g->colliders[id].enabled = true;
    g->colliders[id].type = COLL_BALLOON;
    const Rect br = {
        16, 8, 16, 22
    };
    g->colliders[id].rect = br;

    /* cactus */
	if(reserve)
		entity_reserve(g->entity_roster, cactus_id);

    CLEAR(g->sprites[cactus_id]);
    sprite_set_to(&g->sprites[cactus_id], asset_make_handle(cactus_anim->idle_anim, &g->pak));
    g->sprites[cactus_id].rect.w = asset_from_handle_of(g->sprites[cactus_id].spritesheet, Spritesheet)->width;
    g->sprites[cactus_id].rect.h = asset_from_handle_of(g->sprites[cactus_id].spritesheet, Spritesheet)->height;

    CLEAR(g->transforms[cactus_id]);
    g->transforms[cactus_id].enabled = true;

    CLEAR(g->colliders[cactus_id]);
    g->colliders[cactus_id].enabled = true;
    g->colliders[cactus_id].type = COLL_CACTUS;
    const Rect cr = {
        12, 10, 24, 20
    };
    g->colliders[cactus_id].rect = cr;

    CLEAR(g->cactuses[cactus_id]);
    g->cactuses[cactus_id].enabled = true;
    g->cactuses[cactus_id].anim_table = cactus_anim;

	CLEAR(g->shoots[cactus_id]);
	g->shoots[cactus_id].enabled = true;
	g->shoots[cactus_id].dir.x = 1;
	g->shoots[cactus_id].dir.y = 1;

    /* ai */
    if(!is_player) {
        CLEAR(g->ai[cactus_id]);
        g->ai[id].enabled = true;
        g->ai[id].state = AI_WAITING;
        g->ai[id].timer = 3 * 60;

        g->balloons[id].constrain_to_screen = false;
        g->transforms[id].pos.x = RANDRANGE(400, 700) * (RANDRANGE(0, 1) ? -1 : 1);
        g->transforms[id].pos.x = RANDRANGE(300, 700) * (RANDRANGE(0, 1) ? -1 : 1);
    }

    rope_init(id, 2);
}

void init(void)
{
    const struct PaletteAsset *pal = &g->pak.gamepal;

    assert(pal->col_count <= sizeof(g->palette));
    memcpy(g->palette, pal, pal->col_count);
    renderer_set_palette(g->palette, 0, sizeof(g->palette));

    create_balloon_cactus(EID_PlayerBalloon, EID_PlayerCactus, true, true);
    create_balloon_cactus(EID_Enemies, EID_Enemies + 1, false, true);

    for(int i = 0; i < CLOUD_MAX; ++i) {
		 place_cloud(&g->clouds[i]);
    }
}

void input(void)
{
    struct BalloonComp *balloon = &g->balloons[0];
    balloon->dir.x = 0;
    balloon->dir.y = 0;
    if(keyboard_keys[KEY_D]) {
        balloon->dir.x = 1;
    }
    else if(keyboard_keys[KEY_A]) {
        balloon->dir.x = -1;
    }
    if(keyboard_keys[KEY_W]) {
        balloon->dir.y = -1;
    }
    else if(keyboard_keys[KEY_S]) {
        balloon->dir.y = 1;
    }

	struct ShootComp *shoot = &g->shoots[1];
	if(keyboard_keys[KEY_RIGHT]) {
		shoot->rotation -= SHOOT_INPUT_SPEED;
	}
	else if(keyboard_keys[KEY_LEFT]) {
		shoot->rotation += SHOOT_INPUT_SPEED;
	}

	shoot->dir.x = sin(shoot->rotation);
	shoot->dir.y = cos(shoot->rotation);
	shoot->request_shoot = (byte)keyboard_keys[KEY_SPACE];
}

void update(void)
{
	if(g->game_state == GAME_STATE_BEGIN) {
		struct PaletteAsset *pal = &g->pak.gamepal;
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

	kill_update(0, ENTITY_MAX);
	ai_update(0, ENTITY_MAX);
    balloon_update(0, ENTITY_MAX);
	motor_update(0, ENTITY_MAX);
    transform_update(0, ENTITY_MAX);
    rope_update(0, ENTITY_MAX);
	shoot_update(0, ENTITY_MAX);
    collider_update(0, ENTITY_MAX);
    sprite_update(g->sprites, ENTITY_MAX);

    scroll_clouds();
}

void render(void)
{
    renderer_clear(1);

    for(int i = 0; i < CLOUD_MAX; ++i) {
     	renderer_draw_texture(&g->pak.cloud, g->clouds[i]);
     }

    rope_draw(0, ENTITY_MAX);
    sprite_draw(g->sprites, ENTITY_MAX);
	shoot_draw(EID_PlayerCactus, 1);

#if DEBUG_DRAW
    for(int i = 0; i < ENTITY_MAX; ++i) {
        if(g->colliders[i].enabled && g->transforms[i].enabled) {
            Rect r = g->colliders[i].rect;
            r.x += g->transforms[i].pos.x;
            r.y += g->transforms[i].pos.y;

            renderer_draw_rect(26 + g->colliders[i].type, r);
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

    asset_load_pak((byte *)&g->pak, sizeof(g->pak), "main.dat");
}
