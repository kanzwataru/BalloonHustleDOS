#ifndef CACTOON_H
#define CACTOON_H

#include "src/wrecki~1/consts.h"
#include "src/hustle~1/render.h"

#define BALLOON_STRING_OFFSET 38
#define CACTUS_STRING_OFFSET  16

enum CACTOON_FLAGS {
    CT_ENEMY = 0x01,
    CT_DEAD  = 0x02
};

typedef struct
{
    Sprite      *balloon;
    Sprite      *cactus;
    Point        balloon_vel;
    Point        counters;
    byte         flags;
} CactusBalloon;

void cactoon_init(CactusBalloon *ct, Sprite *balloon, Sprite *cactus);
void cactoon_update(CactusBalloon *ct, byte dir);
void cactoon_die(CactusBalloon *ct);

#endif