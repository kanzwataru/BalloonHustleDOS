#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_CACTOONS        4
#define MAX_BALLOONS        4
#define ROPE_LENGTH         70

#define FIXED_POINT_SHIFT   4

#define GRAVITY             10
#define POINT_BALLOON_SPEED 75
#define PROJECTILE_SPEED    200

#define CACTOON_SPEED       80
#define CACTOON_ACCEL       10
#define CACTOON_DECEL       5

enum WB_DIR {
    WB_LEFT =  0x01,
    WB_RIGHT = 0x02,
    WB_UP =    0x04,
    WB_DOWN =  0x08
};

#endif