#include "events.h"
#include "game.h"

static void balloon_collide(struct Collision self, struct Collision other)
{
    assert(self.id < ENTITY_MAX);
    struct BalloonComp *c = &g->balloons[self.id];
    if(self.collision_type = COLL_BALLOON && c->enabled) {
        c->vel.x = 0;
        c->vel.y = 0; 
    }
}

void event_collide(struct Collision self, struct Collision other)
{
    balloon_collide(self, other);
}

