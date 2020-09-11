#ifndef BH_CONSTS_H
#define BH_CONSTS_H

#define FPS                 60

#define ROPE_SEGMENTS       8
#define ROPE_LENGTH         32
#define ROPE_ITERATIONS     2
#define ROPE_FRICTION       0.95f
#define ROPE_GRAVITY        0.5

#define BALLOON_SPEED       3
#define BALLOON_ACCEL       0.8
#define BALLOON_DECEL       0.85

#define CLOUD_MAX 			 4
#define CLOUD_SPEED			 1
#define CLOUD_HEIGHT_MIN	 300
#define CLOUD_HEIGHT_MAX	 900
#define CLOUD_SIDE_MIN		 -120
#define CLOUD_SIDE_MAX		 200

#define SHOOT_UI_OFFSET      32
#define SHOOT_UI_SIZE        4
#define SHOOT_UI_COL         9
#define SHOOT_INPUT_SPEED    0.06f
#define SHOOT_COOLDOWN       1 * FPS

#define BULLET_SPEED         2.5f

#define AI_DEAD_TIME         2 * FPS

/* DEBUG */
#define DEBUG_DRAW           0

#endif
