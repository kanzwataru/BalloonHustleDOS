#ifndef BH_COMPONENTS_H
#define BH_COMPONENTS_H
#include "hustle.h"
#include "consts.h"

#define NULL_ENTITY 65535
typedef uint16_t entity_id;

typedef struct {
    float x, y;
} FLPoint;

enum ColliderType {
    COLL_DEFAULT,
    COLL_BALLOON,
    COLL_CACTUS
};

enum BalloonState {
    BALLOON_STATE_IDLE,
    BALLOON_STATE_POP
};

enum CactusState {
    CACTUS_STATE_IDLE,
    CACTUS_STATE_FALL
};

enum ShootState {
    SHOOT_STATE_IDLE,
    SHOOT_STATE_FIRE,
    SHOOT_STATE_COOLDOWN
};

enum AIState {
    AI_WAITING,
    AI_MOVING,
    AI_ATTACKING,
    AI_DEAD,
    AI_DEAD_WAITING
};

struct TransformComp {
    bool      enabled;
    FLPoint   pos;
    entity_id parent;
};

struct BalloonComp {
    bool    enabled;
    bool    constrain_to_screen;
    Point   dir;
    FLPoint vel;
    byte    state;

    const struct BalloonAnimTable *anim_table;
};

struct CactusComp {
    bool    enabled;
    bool    constrain_to_screen;
    byte    state;

    const struct CactusAnimTable *anim_table;
};

struct ColliderComp {
    bool    enabled;
    Rect    rect;
    byte    type;
};

struct ShootComp {
    bool    enabled;
    float   rotation;
    FLPoint dir;
    byte    state;
};

struct RopePoint {
    float pos[2];
    float prev_pos[2];
};

struct RopeComp {
    bool enabled;
    byte color;

    struct RopePoint points[ROPE_SEGMENTS];
    Point segments[ROPE_SEGMENTS * 2];

    Point offset;
    Point end_offset;
    entity_id start_transform;
    entity_id end_transform;
};

struct AIComp {
    bool enabled;
    enum AIState state;
    FLPoint  goal;
    uint32_t timer;
};

void rope_init          (entity_id start, entity_id count);

void transform_update   (entity_id start, entity_id count);
void balloon_update     (entity_id start, entity_id count);
void cactus_update      (entity_id start, entity_id count);
void shoot_update       (entity_id start, entity_id count);
void collider_update    (entity_id start, entity_id count);
void rope_update        (entity_id start, entity_id count);
void ai_update          (entity_id start, entity_id count);

void rope_draw          (entity_id start, entity_id count);
void shoot_draw         (entity_id start, entity_id count);

#endif
