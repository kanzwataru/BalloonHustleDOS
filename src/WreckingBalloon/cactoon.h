#ifndef CACTOON_H
#define CACTOON_H

#include "src/hustle~1/render.h"

typedef struct
{
    Sprite *balloon;
    Sprite *cactus;
    Point   balloon_vel;
    Point   cactus_vel;
} CactusBalloon;

void cactoon_init(CactusBalloon *ct, Sprite *balloon, Sprite *cactus);
void cactoon_move(CactusBalloon *ct, byte dir);

#endif