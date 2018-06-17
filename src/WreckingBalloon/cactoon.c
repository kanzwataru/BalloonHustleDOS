#include "src/wrecki~1/cactoon.h"

#include "src/wrecki~1/consts.h"

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

void cactoon_init(CactusBalloon *ct, Sprite *balloon, Sprite *cactus)
{
    memset(ct, 0, sizeof(CactusBalloon));
    ct->balloon = balloon;
    ct->cactus = cactus;
}

void cactoon_move(CactusBalloon *ct, const byte dir)
{
    ct->balloon_vel = calc_velocity(ct->balloon_vel, dir);

    ct->balloon_vel.x = CLAMP(ct->balloon_vel.x, -CACTOON_SPEED, CACTOON_SPEED);
    ct->balloon_vel.y = CLAMP(ct->balloon_vel.y, -CACTOON_SPEED, CACTOON_SPEED);

    ct->balloon->rect.x += ct->balloon_vel.x >> FIXED_POINT_SHIFT;
    ct->balloon->rect.y += ct->balloon_vel.y >> FIXED_POINT_SHIFT;
    
    /* Cactus swing physics */
    ct->cactus_vel.x = ((ct->cactus->rect.x << FIXED_POINT_SHIFT) + ct->balloon_vel.x) - (ct->cactus->rect.x << FIXED_POINT_SHIFT);
    ct->cactus_vel.y = ((ct->cactus->rect.y << FIXED_POINT_SHIFT) + ct->balloon_vel.y) - (ct->cactus->rect.y << FIXED_POINT_SHIFT);

    ct->cactus->rect.x += ct->cactus_vel.x >> FIXED_POINT_SHIFT;
    ct->cactus->rect.y += (ct->cactus_vel.y + GRAVITY) >> FIXED_POINT_SHIFT;
}
