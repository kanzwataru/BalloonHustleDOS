#include <math.h>
#include "particles.h"
#include "vec2d.h"

extern "C" {
#include "engine/render.h"
}

static V2 random_normal()
{
    // don't be silly
    const int x = RANDRANGE(0, 5000);
    const float f = (float)x / 1000.0f;

    V2 n;
    n.x = sin(f);
    n.y = cos(f);
    return n;
}

static Particle *spawn_particle(ParticleSpawner &s, ParticleSystem *sys)
{
    //TEMP: only supports one spawner because we just linearly allocate
    //TODO: should support adding multiple particles at a time
    if(s.particles_count < PRT_MAX_PARTICLES - 1) {
        Particle *p = &sys->particles[s.particles_count++];

        p->pos = V2(s.pos);
        p->vel = random_normal() * 3.0f;

        return p;
    }

    return NULL;
}

static void kill_particle(ParticleSpawner &s, ParticleSystem *sys)
{

}

static inline void sim_particle(Particle &p, const ParticleSpawner &spawner)
{
    p.pos += p.vel;
}

void PRT_update(ParticleSpawner *spawners, uint16_t count, ParticleSystem *sys)
{
    for(uint16_t i = 0; i < count; ++i) {
        ParticleSpawner &s = spawners[i];
        if(!s.flags)
            continue;

        if(s.spawn_counter-- == 0) {
            s.spawn_counter = s.props.rate;

            spawn_particle(s, sys);
        }

        for(particle_id j = s.particles_start; j < s.particles_start + s.particles_count; ++j) {
            sim_particle(sys->particles[j], s);
        }
    }
}

void PRT_draw(ParticleSpawner *spawners, uint16_t count, ParticleSystem *sys)
{
    for(uint16_t i = 0; i < count; ++i) {
        ParticleSpawner &s = spawners[i];

        for(particle_id j = s.particles_start; j < s.particles_start + s.particles_count; ++j) {
            Rect rect;
            rect.x = sys->particles[j].pos.x;
            rect.y = sys->particles[j].pos.y;
            rect.w = 1;
            rect.h = 1;

            renderer_draw_rect(s.palette_end, rect);
        }
    }
}
