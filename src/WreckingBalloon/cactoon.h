#ifndef CACTOON_H
#define CACTOON_H

#include "src/wrecki~1/consts.h"
#include "src/hustle~1/render.h"

#define BALLOON_STRING_OFFSET 38
#define CACTUS_STRING_OFFSET  16

typedef struct
{
    Sprite      *balloon;
    Sprite      *cactus;
    Animation   *idle;
    Animation   *shoot;
    Animation   *fall;
    Point        balloon_vel;
    Point        counters;
} CactusBalloon;

void cactoon_init(CactusBalloon *ct, Sprite *balloon, Sprite *cactus);
void cactoon_move(CactusBalloon *ct, byte dir);

#endif