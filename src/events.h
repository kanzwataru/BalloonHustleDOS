#ifndef BH_EVENTS_H
#define BH_EVENTS_H

#include "components.h"

struct Collision {
    entity_id id;
    byte      collision_type;
};

void event_collide(struct Collision self, struct Collision other);
void event_fire(entity_id self);

#endif
