#include "src/wrecki~1/resource.h"
#include "src/wrecki~1/consts.h" /* TODO: only need the clouds colours, should refactor */

#include "src/hustle~1/filesys.h"
#include "src/hustle~1/rle.h"

#define BALLOON_SIZE        48 * 48
#define CACTUS_SIZE         48 * 48

#define TOTAL_ANIMATIONS    6
#define TOTAL_STILL_SPRITES 1

static Animation *anims[TOTAL_ANIMATIONS];
static buffer_t **images[TOTAL_STILL_SPRITES];

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

RLEImageMono *cloud_image;

/* TODO: convert spritesheets to rle offline!
 *  
 *  too finicky, easy to get wrong,
 *  just write a RLE converter that takes in frame numbers, fps and other parameters,
 *  maybe directly from the Aseprite file
*/
static void load_spritesheet(Animation *anim, const char *path, int size)
{
    buffer_t *raw = (buffer_t *)load_bmp_image(path);
    RLEImage *image = buffer_to_rle(raw);

    anim->frames = image;
    anim->frame_size = size;

    destroy_image(&raw);
}

/* TODO: this is pretty bad in many ways, refactor! */
static RLEImageMono *load_monochrome_sprite(const char *path, byte fgcol, byte bgcol, int width, int height)
{
    buffer_t *raw = (buffer_t *)load_bmp_image(path);
    RLEImageMono *rleimage = monochrome_buffer_to_rle(raw, width, height, fgcol, bgcol);

    destroy_image(&raw);

    return rleimage;
}

void init_all_resources(void)
{
    int i = 0;

                            /* ANIMATION */
    /* ****************************************************************** */
    anims[i++] = &player_balloon_idle;
    load_spritesheet(&player_balloon_idle, "RES\\BLOOA.BMP", BALLOON_SIZE);
    player_balloon_idle.count = 4;
    player_balloon_idle.skip = 6;

    anims[i++] = &player_balloon_pop;
    load_spritesheet(&player_balloon_pop, "RES\\BLOOA-PO.BMP", BALLOON_SIZE);
    player_balloon_pop.count = 8;
    player_balloon_pop.skip = 2;
    player_balloon_pop.playback_type = ANIM_DISAPPEAR;

    anims[i++] = &point_balloon_idle;
    load_spritesheet(&point_balloon_idle, "RES\\BLOOE.BMP", BALLOON_SIZE);
    point_balloon_idle.count = player_balloon_idle.count;
    point_balloon_idle.skip = player_balloon_idle.skip;

    anims[i++] = &point_balloon_pop;
    load_spritesheet(&point_balloon_pop, "RES\\BLOOE-PO.BMP", BALLOON_SIZE);
    point_balloon_pop.count = player_balloon_pop.count;
    point_balloon_pop.skip = player_balloon_pop.skip;
    point_balloon_pop.playback_type = ANIM_DISAPPEAR;

    anims[i++] = &player_cactus_idle;
    load_spritesheet(&player_cactus_idle, "RES\\CACP.BMP", CACTUS_SIZE);
    player_cactus_idle.count = 6;
    player_cactus_idle.skip = 6;

    anims[i++] = &player_cactus_fall;
    load_spritesheet(&player_cactus_fall, "RES\\CACP-FA.BMP", CACTUS_SIZE);
    player_cactus_fall.count = 11;
    player_cactus_fall.skip = 4;
    /* ****************************************************************** */
    /* ****************************************************************** */

                            /* STILL SPRITES */
    /* ****************************************************************** */
    /* ****************************************************************** */
    i = 0;

    //cloud_image = load_bmp_image("RES\\CLOUD.BMP");
    cloud_image = load_monochrome_sprite("RES\\CLOUD.BMP", CLOUD_COL, SKY_COL, CLOUD_SPRITE_W, CLOUD_SPRITE_H);
    images[i++] = &cloud_image;
    /* ****************************************************************** */
    /* ****************************************************************** */
}

void free_all_resources(void)
{
    int i;
    for(i = 0; i < TOTAL_ANIMATIONS; ++i)
        destroy_image(&anims[i]->frames);

    for(i = 0; i < TOTAL_STILL_SPRITES; ++i)
        destroy_image(images + i);
}
