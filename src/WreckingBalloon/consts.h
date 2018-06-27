#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_CACTOONS        4
#define MAX_BALLOONS        4
#define ROPE_SEGMENTS       8
#define ROPE_SEG_LENGTH     8
#define ROPE_ITER_TIMES     1

#define FIXED_POINT_SHIFT   4

#define GRAVITY             10
#define POINT_BALLOON_SPEED 75
#define PROJECTILE_SPEED    200

#define CACTUS_SPEED        50
#define CACTOON_SPEED       100
#define CACTOON_ACCEL       40
#define CACTOON_DECEL       15

enum WB_DIR {
    WB_LEFT =  0x01,
    WB_RIGHT = 0x02,
    WB_UP =    0x04,
    WB_DOWN =  0x08
};

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLAMP(val, min, max) (((val) > (max)) ? (max) : ((val < min)) ? (min) : (val))

#endif