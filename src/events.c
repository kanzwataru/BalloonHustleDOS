#include "events.h"
#include "game.h"

/* local event prototypes */
static void event_balloon_popped(entity_id balloon_id);

/* component-specific event implementations */
static void balloon_collide(struct Collision self, struct Collision other)
{
    assert(self.id < ENTITY_MAX);
    struct BalloonComp *c = &g->balloons[self.id];
    if(self.collision_type == COLL_BALLOON && c->enabled) {
        if(c->state == BALLOON_STATE_IDLE) {
            c->state = BALLOON_STATE_POP;
            sprite_set_to(&g->sprites[self.id], asset_handle_to(BALLOON_POP, Spritesheet, g->pak));

            event_balloon_popped(self.id);
        }
    }
}

static void cactus_balloon_popped(entity_id balloon_id)
{
    entity_id id = balloon_id + 1; /* TODO: find a better way of finding cactus */
    assert(g->cactuses[id].enabled);
    if(g->cactuses[id].state == CACTUS_STATE_IDLE) {
        g->cactuses[id].state = CACTUS_STATE_FALL;
        sprite_set_to(&g->sprites[id], asset_handle_to(CAC_FALL, Spritesheet, g->pak));
    }
}

/* events */
void event_collide(struct Collision self, struct Collision other)
{
    balloon_collide(self, other);
}

static void event_balloon_popped(entity_id balloon_id)
{
    cactus_balloon_popped(balloon_id);
}
