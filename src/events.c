#include "events.h"
#include "game.h"

/* component-specific event implementations */
static void balloon_collide(struct Collision self, struct Collision other)
{
    struct BalloonComp *c = &g->balloons[self.id];

    const bool collider_hurts = other.collision_type == COLL_CACTUS ||
                                other.collision_type == COLL_BULLET;

    if(self.collision_type == COLL_BALLOON && collider_hurts) {
        assert(c->enabled);
        if(c->state == BALLOON_STATE_IDLE) {
            c->state = BALLOON_STATE_POP;
            sprite_set_to(&g->sprites[self.id], asset_make_handle(c->anim_table->pop_anim, &g->pak));

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
        sprite_set_to(&g->sprites[id], asset_make_handle(g->cactuses[id].anim_table->fall_anim, &g->pak));
    }
}

static void ai_balloon_popped(entity_id balloon_id)
{
    if(g->ai[balloon_id].enabled) {
        g->ai[balloon_id].state = AI_DEAD;
    }
}

/* events */
void event_collide(struct Collision self, struct Collision other)
{
    balloon_collide(self, other);
}

void event_balloon_popped(entity_id balloon_id)
{
    cactus_balloon_popped(balloon_id);
    ai_balloon_popped(balloon_id);
}
