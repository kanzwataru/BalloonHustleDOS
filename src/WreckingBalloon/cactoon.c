#include "src/wrecki~1/cactoon.h"
#include "src/hustle~1/math.h"

#include <stdio.h>
#include <dos.h>

#define TRAILING_SIDE_OFFSET_X      45
#define TRAILING_SIDE_OFFSET_Y      25
#define TRAILING_VERTICAL_OFFSET    35
#define TRAILING_DECAY              1
#define TRAILING_ACCEL              5

#define TOWARDS_ZERO(a, amnt)     (a) = ((a) <= 0) ? 0 : ((a) - (amnt))
#define TOWARDS_MAX(a, max, amnt) (a) = ((a) >= (max)) ? (max) : ((a) + (amnt))

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

typedef struct {
    int left;
    int right;
    int up;
    int down;
} Counters;

static void simple_physics(Rect *cact_rect, const byte dir)
{
    static Counters counters;

    if(dir == 0) {
        TOWARDS_ZERO(counters.left,  TRAILING_DECAY);
        TOWARDS_ZERO(counters.right, TRAILING_DECAY);
        TOWARDS_ZERO(counters.up,    TRAILING_DECAY);
        TOWARDS_ZERO(counters.down,  TRAILING_DECAY);
    }

    if(dir & WB_LEFT) {
        TOWARDS_ZERO(counters.left, TRAILING_ACCEL);

        TOWARDS_MAX(counters.right, TRAILING_SIDE_OFFSET_X, TRAILING_ACCEL);
        TOWARDS_MAX(counters.up,    TRAILING_SIDE_OFFSET_Y, TRAILING_ACCEL);
    }
    else if(dir & WB_RIGHT) {
        TOWARDS_ZERO(counters.right, TRAILING_ACCEL);

        TOWARDS_MAX(counters.left,  TRAILING_SIDE_OFFSET_X, TRAILING_ACCEL);
        TOWARDS_MAX(counters.up,    TRAILING_SIDE_OFFSET_Y, TRAILING_ACCEL);
    }

    if(dir & WB_UP) {
        TOWARDS_ZERO(counters.up, TRAILING_ACCEL);

        TOWARDS_MAX(counters.down,  TRAILING_VERTICAL_OFFSET, TRAILING_ACCEL);
    }
    else if(dir & WB_DOWN) {
        TOWARDS_ZERO(counters.down, TRAILING_ACCEL);

        TOWARDS_MAX(counters.up,    TRAILING_VERTICAL_OFFSET, TRAILING_ACCEL);
    }

    cact_rect->x = counters.right - counters.left;
    cact_rect->y = ROPE_LENGTH + (counters.down - counters.up);
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
