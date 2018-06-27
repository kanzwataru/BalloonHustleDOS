#include "src/wrecki~1/cactoon.h"
#include "src/hustle~1/vector.h"

#include <stdio.h>
#include <dos.h>

#define top segs[i]
#define btm segs[i + 1]

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

static void fancy_constraint(RopeSegment *segs)
{
    int i, _;
    float dist, dist_error;
    Vector2D change_dir, change, velocity;

    /* rope length constraint */
    for(_ = 0; _ < ROPE_ITER_TIMES; ++_) {
                            //printf("   CONSTRAINT\n", _);
    for(i = 0; i < ROPE_SEGMENTS - 1; ++i) {

        //printf("(%.2f) {%.2f %.2f}, {%.2f %.2f}\n", i, top.pos.x, top.pos.y, btm.pos.x, btm.pos.y);

        dist = vmagnitude(vsubv(btm.pos, top.pos));
        dist_error = abs(dist - ROPE_SEG_LENGTH);

        if(dist > ROPE_SEG_LENGTH)
            change_dir = vnormalize(vsubv(top.pos, btm.pos));
        else if(dist < ROPE_SEG_LENGTH)
            change_dir = vnormalize(vsubv(btm.pos, top.pos));
        else
            continue;

        change = vmuli(change_dir, dist_error);

        if(i != 0) {
            btm.pos = vaddv(btm.pos, vdivi(change, 2));
            top.pos = vaddv(top.pos, vdivi(change, 2));
        }
        else {
            btm.pos = vaddv(btm.pos, change);
        }

        //printf("     {%.2f %.2f}, {%.2f %.2f}\n", top.pos.x, top.pos.y, btm.pos.x, btm.pos.y);
    }
        //getch();
    }
}

static void simple_constraint(RopeSegment *segs)
{
    int i;
    int dist;
    int dist_error;
    for(i = 0; i < ROPE_SEGMENTS - 1; ++i) {
        dist = (btm.pos.y - top.pos.y) + (btm.pos.x - top.pos.x);
        dist_error = abs(dist - ROPE_SEG_LENGTH);

        //printf("dist: %d", dist);
        //printf("dist_error: %d\n", dist_error);

        if(dist > ROPE_SEG_LENGTH) { /* too long */
            btm.pos.y -= dist_error / 2;
            btm.pos.x += dist_error / 2;
        }
        else if(dist < ROPE_SEG_LENGTH) {/* too short */
            btm.pos.y += dist_error / 2;
            btm.pos.x -= dist_error / 2;
        }
        else /* ok */
            continue;
    }

            //getch();
}

static void swing_physics(RopeSegment *segs)
{
    int i, _;
    Vector2D velocity;

    /* Verlet */
    //printf("    VERLET\n");
    for(i = 0; i < ROPE_SEGMENTS; ++i) {
        //printf("(%.2f) {%.2f %.2f}", i, segs[i].pos.x, segs[i].pos.y);
        velocity = vsubv(segs[i].pos, segs[i].oldpos);
        segs[i].oldpos = segs[i].pos;
        segs[i].pos = vaddv(segs[i].pos, velocity);
        segs[i].pos.y += GRAVITY;
        //printf(" ==> {%.2f %.2f}\n", segs[i].pos.x, segs[i].pos.y);
    }

    //simple_constraint(segs);
    fancy_constraint(segs);
}

void cactoon_init(CactusBalloon *ct, Sprite *balloon, Sprite *cactus)
{
    int i;
    Vector2D pos;
    pos.x = balloon->rect.x;
    pos.y = balloon->rect.y;

    memset(ct, 0, sizeof(CactusBalloon));
    ct->balloon = balloon;
    ct->cactus = cactus;

    for(i = 0; i < ROPE_SEGMENTS; ++i) {
        ct->segs[i].pos = pos;
        ct->segs[i].oldpos = pos;

        pos.y += ROPE_SEG_LENGTH;
    }
}

void cactoon_move(CactusBalloon *ct, const byte dir)
{
    ct->balloon_vel = calc_velocity(ct->balloon_vel, dir);

    ct->balloon_vel.x = CLAMP(ct->balloon_vel.x, -CACTOON_SPEED, CACTOON_SPEED);
    ct->balloon_vel.y = CLAMP(ct->balloon_vel.y, -CACTOON_SPEED, CACTOON_SPEED);

    ct->balloon->rect.x += ct->balloon_vel.x >> FIXED_POINT_SHIFT;
    ct->balloon->rect.y += ct->balloon_vel.y >> FIXED_POINT_SHIFT;
    
    /* Cactus swing physics */
    ct->segs[0].pos.x = ct->balloon->rect.x;
    ct->segs[0].pos.y = ct->balloon->rect.y;

    swing_physics(ct->segs);

    ct->cactus->rect.x = ct->segs[ROPE_SEGMENTS - 1].pos.x;
    ct->cactus->rect.y = ct->segs[ROPE_SEGMENTS - 1].pos.y;
}
