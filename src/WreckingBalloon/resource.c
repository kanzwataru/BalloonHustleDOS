#include "src/wrecki~1/resource.h"
#include "src/hustle~1/filesys.h"

#define BALLOON_SIZE        48 * 48
#define CACTUS_SIZE         48 * 48
#define TOTAL_ANIMATIONS    2

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
    player_balloon_idle.frames = load_bmp_image("RES\\BOON-A.BMP");
    player_balloon_idle.frame_size = BALLOON_SIZE;
    player_balloon_idle.count = 4;
    player_balloon_idle.skip = 6;

    anims[i++] = &player_cactus_idle;
    player_cactus_idle.frames = load_bmp_image("RES\\CACTUS-A.BMP");
    player_cactus_idle.frame_size = CACTUS_SIZE;
    player_cactus_idle.count = 6;
    player_cactus_idle.skip = 6;
}

void free_all_resources(void)
{
    int i;
    for(i = 0; i < TOTAL_ANIMATIONS; ++i)
        destroy_image(&anims[i]->frames);
}