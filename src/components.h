#ifndef BH_COMPONENTS_H
#define BH_COMPONENTS_H
#include "hustle.h"
#include "consts.h"

#define NULL_ENTITY 65535
typedef uint16_t entity_id;

typedef struct {
    float x, y;
} FLPoint;

struct TransformComp {
    bool      enabled;
    FLPoint   pos;
    entity_id parent;
};

struct BalloonComp {
    bool    enabled;
    Point   dir;
    FLPoint vel;
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

void rope_init(entity_id start, entity_id count);

void transform_update(entity_id start, entity_id count);
void balloon_update(entity_id start, entity_id count);
void rope_update(entity_id start, entity_id count);

void rope_draw(entity_id start, entity_id count);

#endif

