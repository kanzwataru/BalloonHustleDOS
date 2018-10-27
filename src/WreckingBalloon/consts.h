#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FIXED_POINT_SHIFT   4
#define FP /* unit should be fixed point */

#define MAX_CACTOONS        3
#define MAX_BALLOONS        4
#define MAX_CLOUDS          3

#define ROPE_LENGTH         1100 FP
#define CLOUDS_FRAME_SKIP   1
#define CLOUDS_SPEED        1

#define POINT_BALLOON_SPEED 3
#define PROJECTILE_SPEED    200 FP

#define CACTUS_FALL_SPEED   3
#define CACTOON_SPEED       80 FP
#define CACTOON_ACCEL       10 FP
#define CACTOON_DECEL       5 FP

#define BALLOON_HITBOX      {10,10,28,28}
#define CACTUS_HITBOX       {10,10,24,24}

#define POINT_BALLOON_POINTS  50
#define CACTOON_POINTS        150

enum WB_DIR {
    WB_STILL = 0x00,
    WB_LEFT  = 0x01,
    WB_RIGHT = 0x02,
    WB_UP    = 0x04,
    WB_DOWN  = 0x08
};

/* Colour palette defines */
#define STRING_COL          23
#define SKY_COL             1
#define CLOUD_COL           3
/* */

#define MAX_SPRITES (MAX_CACTOONS * 2) + (MAX_BALLOONS) + (MAX_CLOUDS)

#endif
