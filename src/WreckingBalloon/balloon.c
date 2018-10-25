#include "balloon.h"
#include "resource.h"
#include "engine/event.h"
#include "consts.h"

#include <stdlib.h>

static void reset_pos(void *self)
{
    (*(PointBalloon *)self).sprite->rect.x = rand() % 300;
    (*(PointBalloon *)self).sprite->rect.y = 300 + (rand() % 700);
    (*(PointBalloon *)self).sprite->anim.animation = &point_balloon_idle;
    (*(PointBalloon *)self).popped = false;
}

void balloon_init(PointBalloon *pb, Sprite *sprite, uint16 points) 
{
    Rect hitbox = BALLOON_HITBOX;
    pb->sprite = sprite;
    pb->points = points;
    srand((size_t)pb);

    sprite->rect.x = rand() % 200;
    reset_pos(pb);
    sprite->rect.w = CACTOON_SPRITE_SIZE;
    sprite->rect.h = CACTOON_SPRITE_SIZE;
    sprite->flags = SPRITE_REFRESH | SPRITE_CLIP | SPRITE_MASKED;
    sprite->hitbox = hitbox;
}

void balloon_pop(PointBalloon *pb)
{
    if(pb->popped)
        return;

    pb->popped = true;
    pb->sprite->anim.animation = &point_balloon_pop;
    pb->sprite->anim.frame = 0;
    event_add(&reset_pos, pb, point_balloon_pop.count * point_balloon_pop.skip);
}

void balloon_update(PointBalloon *pb) 
{
    if(pb->popped)
        pb->sprite->rect.y -= 1;
    else
        pb->sprite->rect.y -= POINT_BALLOON_SPEED;
    
    if(pb->sprite->rect.y < -300)
        reset_pos(pb);
}