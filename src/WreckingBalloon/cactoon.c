#include "src/wrecki~1/cactoon.h"
#include "src/wrecki~1/resource.h"
#include "src/hustle~1/math.h"

#include <stdio.h>
#include <dos.h>

#define TRAILING_SIDE_OFFSET_X      800
#define TRAILING_SIDE_OFFSET_Y      300
#define TRAILING_VERTICAL_OFFSET    850
#define TRAILING_DECAY              15
#define TRAILING_ACCEL              45

#define TOWARDS_ZERO(a, amnt)  (a) = ((a) > (amnt)) ? ((a) - (amnt)) : ((a) < -(amnt)) ? ((a) + (amnt)) : 0
#define INCREASE(a, max, amnt) (a) = ((a) >= (max)) ? (max) : ((a) + (amnt))
#define DECREASE(a, min, amnt) (a) = ((a) <= (min)) ? (min) : ((a) - (amnt))

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

static Point simple_physics(Point counters, const byte dir)
{
    if(dir == 0) {
        TOWARDS_ZERO(counters.x, TRAILING_DECAY);
        TOWARDS_ZERO(counters.y, TRAILING_DECAY);
    }

    if(dir & WB_LEFT) {
        INCREASE(counters.x, TRAILING_SIDE_OFFSET_X, TRAILING_ACCEL);
        DECREASE(counters.y, -TRAILING_SIDE_OFFSET_Y, TRAILING_ACCEL);
    }
    else if(dir & WB_RIGHT) {
        DECREASE(counters.x, -TRAILING_SIDE_OFFSET_X, TRAILING_ACCEL);
        DECREASE(counters.y, -TRAILING_SIDE_OFFSET_Y, TRAILING_ACCEL);
    }

    if(dir & WB_UP) {
        INCREASE(counters.y, TRAILING_VERTICAL_OFFSET, TRAILING_ACCEL);
    }
    else if(dir & WB_DOWN) {
        DECREASE(counters.y, -TRAILING_VERTICAL_OFFSET, TRAILING_ACCEL);
    }

    return counters;
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

void cactoon_update(CactusBalloon *ct, const byte dir)
{
    if(ct->flags & CT_DEAD) {
        ct->cactus->rect.y += CACTUS_FALL_SPEED;
        return;
    }

    ct->balloon_vel = calc_velocity(ct->balloon_vel, dir);

    ct->balloon_vel.x = CLAMP(ct->balloon_vel.x, -CACTOON_SPEED, CACTOON_SPEED);
    ct->balloon_vel.y = CLAMP(ct->balloon_vel.y, -CACTOON_SPEED, CACTOON_SPEED);

    ct->balloon->rect.x += ct->balloon_vel.x >> FIXED_POINT_SHIFT;
    ct->balloon->rect.y += ct->balloon_vel.y >> FIXED_POINT_SHIFT;
    
    /* Cactus swing physics */
    ct->counters = simple_physics(ct->counters, dir);
    ct->cactus->rect.x = ct->balloon->rect.x + (ct->counters.x >> FIXED_POINT_SHIFT);
    ct->cactus->rect.y = ct->balloon->rect.y + ((ROPE_LENGTH + ct->counters.y) >> FIXED_POINT_SHIFT);
}

void cactoon_die(CactusBalloon *ct)
{
    ct->flags |= CT_DEAD;

    if(ct->flags & CT_ENEMY) {
        ct->balloon->anim.animation = &enemy_balloon_pop;
        ct->cactus->anim.animation = &enemy_cactus_fall;
    }
    else {
        ct->balloon->anim.animation = &player_balloon_pop;
        ct->cactus->anim.animation = &player_cactus_fall;
    }
}
