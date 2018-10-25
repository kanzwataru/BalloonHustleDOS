#ifndef RESOURCE_H
#define RESOURCE_H

#include "engine/render.h"

void init_all_resources(void);
void free_all_resources(void);

#define CACTOON_SPRITE_SIZE 48
#define CACTOON_SPRITE_HALF 24 /* 48 / 2 */
#define CLOUD_SPRITE_W      128
#define CLOUD_SPRITE_H      48

extern Animation player_balloon_idle;
extern Animation player_balloon_pop;
extern Animation player_cactus_idle;
extern Animation player_cactus_fall;
extern Animation player_cactus_shoot;

extern Animation enemy_balloon_idle;
extern Animation enemy_balloon_pop;
extern Animation enemy_cactus_idle;
extern Animation enemy_cactus_fall;
extern Animation enemy_cactus_shoot;

extern Animation point_balloon_idle;
extern Animation point_balloon_pop;
extern Animation bomb_balloon_idle;
extern Animation bomb_balloon_pop;

extern buffer_t *cloud_image;

#endif
