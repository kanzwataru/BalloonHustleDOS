#ifndef BH_COMPONENTS_H
#define BH_COMPONENTS_H
#include "hustle.h"
#include "consts.h"

typedef uint16_t entity_id;

struct TransformComp {
    bool      enabled;
    Point     pos;
    Point     dir;
    entity_id parent;
};

struct BalloonComp {
    int _;    
};

struct RopePoint {
    Point pos;
    Point prev_pos;
};

struct RopeComp {
    bool enabled;
    struct RopePoint points[ROPE_SEGMENTS];
    Point segments[ROPE_SEGMENTS * 2];    
    entity_id end_tranform;
};

void transform_update(struct TransformComp *c, size_t count);
void balloon_update(struct BalloonComp *c, size_t count);
void rope_update(struct RopeComp *c, size_t count);

#endif

