#include "src/wrecki~1/cactoon.h"
#include "src/hustle~1/math.h"

#include <stdio.h>
#include <dos.h>

#define TRAILING_SIDE_OFFSET_X      35
#define TRAILING_SIDE_OFFSET_Y      10
#define TRAILING_VERTICAL_OFFSET    35

static Point calc_velocity(Point vel, const byte dir)
{
    if(dir == 0) {
        if(vel.x > 0) {
            vel.x -= CACTOON_DECEL;
            if(vel.x < 0)
                vel.x = 0;
        }
        else {
            vel.x += CACTOON_DECEL;
            if(vel.x > 0)
                vel.x = 0;
        }
        
        if(vel.y > 0) {
            vel.y -= CACTOON_DECEL;
            if(vel.y < 0)
                vel.y = 0;
        }
        else {
            vel.y += CACTOON_DECEL;
            if(vel.y > 0)
                vel.y = 0;
        }
    }

    if(dir & WB_LEFT)
        vel.x -= CACTOON_ACCEL;
    else if(dir & WB_RIGHT)
        vel.x += CACTOON_ACCEL;

    if(dir & WB_DOWN)
        vel.y += CACTOON_ACCEL;
    else if(dir & WB_UP)
        vel.y -= CACTOON_ACCEL;

    return vel;
}

static void simple_physics(Rect *cact_rect, const byte dir)
{
    cact_rect->y = ROPE_LENGTH;

    if(dir == 0) {
        cact_rect->x = 0;
        cact_rect->y = ROPE_LENGTH;
    }

    if(dir & WB_LEFT) {
        cact_rect->x = TRAILING_SIDE_OFFSET_X;
        cact_rect->y -= TRAILING_SIDE_OFFSET_Y;
    }
    else if(dir & WB_RIGHT) {
        cact_rect->x = -TRAILING_SIDE_OFFSET_X;
        cact_rect->y -= TRAILING_SIDE_OFFSET_Y;
    }

    if(dir & WB_UP) {
        cact_rect->y += TRAILING_VERTICAL_OFFSET;
    }
    else if(dir & WB_DOWN) {
        cact_rect->y -= TRAILING_VERTICAL_OFFSET;
    }
}

void cactoon_init(CactusBalloon *ct, Sprite *balloon, Sprite *cactus)
{
    int i;

    memset(ct, 0, sizeof(CactusBalloon));
    ct->balloon = balloon;
    ct->cactus = cactus;

    cactus->rect.x = 0;
    cactus->rect.y = ROPE_LENGTH;
}

void cactoon_move(CactusBalloon *ct, const byte dir)
{
    ct->balloon_vel = calc_velocity(ct->balloon_vel, dir);

    ct->balloon_vel.x = CLAMP(ct->balloon_vel.x, -CACTOON_SPEED, CACTOON_SPEED);
    ct->balloon_vel.y = CLAMP(ct->balloon_vel.y, -CACTOON_SPEED, CACTOON_SPEED);

    ct->balloon->rect.x += ct->balloon_vel.x >> FIXED_POINT_SHIFT;
    ct->balloon->rect.y += ct->balloon_vel.y >> FIXED_POINT_SHIFT;
    
    /* Cactus swing physics */
    simple_physics(&ct->cactus->rect, dir);
}
