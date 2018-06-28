#include "src/wrecki~1/resource.h"
#include "src/hustle~1/filesys.h"

#define BALLOON_SIZE        48 * 48
#define CACTUS_SIZE         48 * 48
#define TOTAL_ANIMATIONS    6

static Animation *anims[TOTAL_ANIMATIONS];

Animation player_balloon_idle;
Animation player_balloon_pop;
Animation player_cactus_idle;
Animation player_cactus_fall;
Animation player_cactus_shoot;

Animation enemy_balloon_idle;
Animation enemy_balloon_pop;
Animation enemy_cactus_idle;
Animation enemy_cactus_fall;
Animation enemy_cactus_shoot;

Animation point_balloon_idle;
Animation point_balloon_pop;
Animation bomb_balloon_idle;
Animation bomb_balloon_pop;

void init_all_resources(void)
{
    int i = 0;

    anims[i++] = &player_balloon_idle;
    player_balloon_idle.frames = load_bmp_image("RES\\BLOOA.BMP");
    player_balloon_idle.frame_size = BALLOON_SIZE;
    player_balloon_idle.count = 4;
    player_balloon_idle.skip = 6;

    anims[i++] = &player_balloon_pop;
    player_balloon_pop.frames = load_bmp_image("RES\\BLOOA-PO.BMP");
    player_balloon_pop.frame_size = BALLOON_SIZE;
    player_balloon_pop.count = 8;
    player_balloon_pop.skip = 2;
    player_balloon_pop.playback_type = ANIM_DISAPPEAR;

    anims[i++] = &point_balloon_idle;
    point_balloon_idle.frames = load_bmp_image("RES\\BLOOE.BMP");
    point_balloon_idle.frame_size = BALLOON_SIZE;
    point_balloon_idle.count = player_balloon_idle.count;
    point_balloon_idle.skip = player_balloon_idle.skip;

    anims[i++] = &point_balloon_pop;
    point_balloon_pop.frames = load_bmp_image("RES\\BLOOE-PO.BMP");
    point_balloon_pop.frame_size = BALLOON_SIZE;
    point_balloon_pop.count = player_balloon_pop.count;
    point_balloon_pop.skip = player_balloon_pop.skip;
    point_balloon_pop.playback_type = ANIM_DISAPPEAR;

    anims[i++] = &player_cactus_idle;
    player_cactus_idle.frames = load_bmp_image("RES\\CACP.BMP");
    player_cactus_idle.frame_size = CACTUS_SIZE;
    player_cactus_idle.count = 6;
    player_cactus_idle.skip = 6;

    anims[i++] = &player_cactus_fall;
    player_cactus_fall.frames = load_bmp_image("RES\\CACP-FA.BMP");
    player_cactus_fall.frame_size = CACTUS_SIZE;
    player_cactus_fall.count = 11;
    player_cactus_fall.skip = 4;
}

void free_all_resources(void)
{
    int i;
    for(i = 0; i < TOTAL_ANIMATIONS; ++i)
        destroy_image(&anims[i]->frames);
}