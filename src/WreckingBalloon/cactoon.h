#ifndef CACTOON_H
#define CACTOON_H

#include "src/wrecki~1/consts.h"
#include "src/hustle~1/render.h"

typedef struct
{
    Vector2D pos;
    Vector2D oldpos;
} RopeSegment;

typedef struct
{
    Sprite      *balloon;
    Sprite      *cactus;
    Animation   *idle;
    Animation   *shoot;
    Animation   *fall;
    Point        balloon_vel;
    Point        cactus_vel;
    RopeSegment  segs[ROPE_SEGMENTS];
} CactusBalloon;

void cactoon_init(CactusBalloon *ct, Sprite *balloon, Sprite *cactus);
void cactoon_move(CactusBalloon *ct, byte dir);

#endif