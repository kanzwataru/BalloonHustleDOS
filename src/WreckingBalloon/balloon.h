#ifndef BALLOON_H
#define BALLOON_H

#include "engine/render.h"

typedef struct {
    Sprite *sprite;
    uint    points;
    bool    popped;
} PointBalloon;

void balloon_init(PointBalloon *pb, Sprite *sprite, uint points);
void balloon_pop(PointBalloon *pb);
void balloon_update(PointBalloon *pb);

#endif
