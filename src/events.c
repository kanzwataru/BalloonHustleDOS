#include "events.h"
#include "game.h"

static void balloon_collide(struct Collision self, struct Collision other)
{
    assert(self.id < ENTITY_MAX);
    struct BalloonComp *c = &g->balloons[self.id];
    if(self.collision_type == COLL_BALLOON && c->enabled) {
        if(c->state == BALLOON_STATE_IDLE) {
            c->state = BALLOON_STATE_POP;
            sprite_set_to(&g->sprites[self.id], asset_handle_to(BALLOON_POP, Spritesheet, g->pak));
        }
    }
}

void event_collide(struct Collision self, struct Collision other)
{
    balloon_collide(self, other);
}

