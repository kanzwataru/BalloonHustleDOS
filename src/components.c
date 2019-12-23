#include "components.h"
#include "game.h"
#include "events.h"

#include <math.h>

void transform_update(entity_id start, entity_id count) 
{
    entity_id id;
    for(id = start; id < count; ++id) {
        struct TransformComp *c = &g->transforms[id];
        if(!c->enabled) continue;

        if(c->parent) { /* TODO: make distinction between null entity and first entity */
            g->sprites[id].rect.x = c->pos.x + g->transforms[c->parent].pos.x;
            g->sprites[id].rect.y = c->pos.y + g->transforms[c->parent].pos.y;
        }
        else {
            g->sprites[id].rect.x = c->pos.x;
            g->sprites[id].rect.y = c->pos.y;
        }
    }
}

void balloon_update(entity_id start, entity_id count)
{
    entity_id id;
    for(id = start; id < count; ++id) {
        struct BalloonComp *c = &g->balloons[id];
        if(!c->enabled) continue;
        assert(g->transforms[id].enabled);

        switch(c->state) {
        case BALLOON_STATE_IDLE:
            c->vel.x *= BALLOON_DECEL;
            c->vel.y *= BALLOON_DECEL;

            c->vel.x += c->dir.x * BALLOON_ACCEL;
            c->vel.y += c->dir.y * BALLOON_ACCEL;

            g->transforms[id].pos.x += c->vel.x;
            g->transforms[id].pos.y += c->vel.y;

            if(c->constrain_to_screen) {
                g->transforms[id].pos.x = CLAMP(g->transforms[id].pos.x, 0, 280);
                g->transforms[id].pos.y = CLAMP(g->transforms[id].pos.y, 0, 120);
            }
            break;
        case BALLOON_STATE_POP:
            c->vel.x *= BALLOON_DECEL;
            c->vel.y *= BALLOON_DECEL;
            c->vel.y += BALLOON_ACCEL;

            g->transforms[id].pos.x += c->vel.x;
            g->transforms[id].pos.y += c->vel.y;
            break; 
        default:
            assert(0);
        }
    }
}

void cactus_update(entity_id start, entity_id count)
{
   /*
   for(entity_id id = 0; id < count; ++id)
   {
        struct CactusComp *c = &g->cactuses[id];
        if(!c->enabled) continue;

   } 
   */
}

void collider_update(entity_id start, entity_id count)
{
    for(entity_id a_id = 0; a_id < count; ++a_id) {
        if(!g->colliders[a_id].enabled) continue;
 
        Rect a = g->colliders[a_id].rect;
        a.x += g->transforms[a_id].pos.x;
        a.y += g->transforms[a_id].pos.y;

        for(entity_id b_id = 0; b_id < count; ++b_id) {
            Rect b = g->colliders[b_id].rect;
            b.x += g->transforms[b_id].pos.x;
            b.y += g->transforms[b_id].pos.y;       
            
            if(g->colliders[b_id].enabled &&
               a.x < b.x + b.w && a.x + a.w > b.x &&
               a.y < b.y && a.y + a.h > b.y)
            {
                struct Collision self = {a_id, g->colliders[a_id].type};
                struct Collision other = {b_id, g->colliders[b_id].type};
                event_collide(self, other);
            }
        }
    }
}

void rope_init(entity_id start, entity_id count)
{
    entity_id id;
    int i;
    Point pos;
    for(id = start; id < count; ++id) {
        struct RopeComp *c = &g->ropes[id];
        if(!c->enabled) continue;
        assert(g->transforms[c->start_transform].enabled);
        pos.x = g->transforms[c->start_transform].pos.x + c->offset.x;
        pos.y = g->transforms[c->start_transform].pos.y + c->offset.y;

        for(i = 0; i < ROPE_SEGMENTS; ++i) {
            c->points[i].pos[0] = pos.x;
            c->points[i].pos[1] = pos.y;

            c->points[i].prev_pos[0] = pos.x;
            c->points[i].prev_pos[1] = pos.y;

            /* TODO: don't have this be seperate */
            c->segments[(i * 2) + 0].x = pos.x;
            c->segments[(i * 2) + 0].y = pos.y;

            c->segments[(i * 2) + 1].x = pos.x;
            c->segments[(i * 2) + 1].y = pos.y;

            pos.y += ROPE_LENGTH / ROPE_SEGMENTS;
        }
    }
}

void rope_update(entity_id start, entity_id count)
{
    entity_id id;
    int i;
    struct Point pos;

    for(id = start; id < count; ++id) {
        struct RopeComp *c = &g->ropes[id];
        if(!c->enabled) continue;
        assert(g->transforms[c->start_transform].enabled);
        pos.x = g->transforms[c->start_transform].pos.x + c->offset.x;
        pos.y = g->transforms[c->start_transform].pos.y + c->offset.y;

        c->points[0].pos[0] = pos.x;
        c->points[0].pos[1] = pos.y;
        
        /* copy (TODO: don't do this) */
        c->segments[0].x = pos.x;
        c->segments[0].y = pos.y;

        const float length = ROPE_LENGTH / ROPE_SEGMENTS;

        for(i = 1; i < ROPE_SEGMENTS; ++i) {
            /* gravity */
            c->points[i].pos[1] += ROPE_GRAVITY;
        }
        
        for(int j = 0; j < ROPE_ITERATIONS; ++j) {
            /* constraints */
            for(i = 1; i < ROPE_SEGMENTS; ++i) {
                float dx = c->points[i].pos[0] - c->points[i - 1].pos[0];
                float dy = c->points[i].pos[1] - c->points[i - 1].pos[1];
                float dist = sqrt(dx * dx + dy * dy);
                float offset = (length - dist) / dist;

                if(i == 1) {
                    c->points[i].pos[0] += dx * offset;
                    c->points[i].pos[1] += dy * offset;
                }
                else {
                    c->points[i - 1].pos[0] -= dx * 0.5f * offset;
                    c->points[i - 1].pos[1] -= dy * 0.5f * offset;
                    c->points[i].pos[0] += dx * 0.5f * offset;
                    c->points[i].pos[1] += dy * 0.5f * offset;
                }
            }
        }

        for(i = 1; i < ROPE_SEGMENTS; ++i) {
            /* verlet integration */
            float posx = c->points[i].pos[0];
            float posy = c->points[i].pos[1];
            c->points[i].pos[0] += ROPE_FRICTION * (c->points[i].pos[0] - c->points[i].prev_pos[0]);
            c->points[i].pos[1] += ROPE_FRICTION * (c->points[i].pos[1] - c->points[i].prev_pos[1]);
            c->points[i].prev_pos[0] = posx;
            c->points[i].prev_pos[1] = posy;

            /* copy (TODO: don't do this) */
            c->segments[(i * 2) - 1].x = c->points[i].pos[0];
            c->segments[(i * 2) - 1].y = c->points[i].pos[1];

            c->segments[(i * 2)].x = c->points[i].pos[0];
            c->segments[(i * 2)].y = c->points[i].pos[1];
        }

        if(c->end_transform) {
            assert(g->transforms[c->end_transform].enabled);
            g->transforms[c->end_transform].pos.x = c->points[ROPE_SEGMENTS - 1].pos[0] + c->end_offset.x;
            g->transforms[c->end_transform].pos.y = c->points[ROPE_SEGMENTS - 1].pos[1] + c->end_offset.y;
        }
    }
}

void rope_draw(entity_id start, entity_id count)
{
    entity_id id;
    struct RopeComp *c;
    for(id = start; id < count; ++id) {
        c = &g->ropes[id];
        if(!c->enabled) continue;

        renderer_draw_line(c->color, c->segments, (ROPE_SEGMENTS * 2) - 1);
    }
}

