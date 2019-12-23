#include "components.h"
#include "game.h"

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

        g->transforms[id].pos.x += c->dir.x * BALLOON_SPEED;
        g->transforms[id].pos.y += c->dir.y * BALLOON_SPEED;
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
        pos = g->transforms[c->start_transform].pos;
        pos.x += c->offset.x;
        pos.y += c->offset.y;

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
        pos = g->transforms[c->start_transform].pos;
        pos.x += c->offset.x;
        pos.y += c->offset.y;

        c->points[0].pos[0] = pos.x;
        c->points[0].pos[1] = pos.y;
        
        /* copy (TODO: don't do this) */
        c->segments[0].x = pos.x;
        c->segments[0].y = pos.y;

        const float length = ROPE_LENGTH / ROPE_SEGMENTS;

        for(i = 1; i < ROPE_SEGMENTS; ++i) {
            /* gravity */
            c->points[i].pos[1] += ROPE_GRAVITY;

            /* constraints */
            for(int j = 0; j < 16; ++j) {
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

            /* verlet integration */
            float posx = c->points[i].pos[0];
            float posy = c->points[i].pos[1];
            c->points[i].pos[0] += c->points[i].pos[0] - c->points[i].prev_pos[0];
            c->points[i].pos[1] += c->points[i].pos[1] - c->points[i].prev_pos[1];
            c->points[i].prev_pos[0] = posx;
            c->points[i].prev_pos[1] = posy;

            /* copy (TODO: don't do this) */
            c->segments[(i * 2) - 1].x = c->points[i].pos[0];
            c->segments[(i * 2) - 1].y = c->points[i].pos[1];

            c->segments[(i * 2)].x = c->points[i].pos[0];
            c->segments[(i * 2)].y = c->points[i].pos[1];
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

        renderer_draw_line(c->color, c->segments, ROPE_SEGMENTS * 2);
    }
}

