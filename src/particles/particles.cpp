#include <math.h>
#include <stdlib.h>
#include "particles.h"
#include "vec2d.h"

extern "C" {
#include "engine/render.h"
}

#define ITER_PARTICLES(_var, _spawner) \
    for(particle_id _var = _spawner.particles_start; _var < _spawner.particles_start + _spawner.particles_count; ++_var)

#define ITER_PARTICLES_OFFSET(_var, _spawner, _b, _e) \
    for(particle_id _var = _spawner.particles_start + (_b); _var < _spawner.particles_start + _spawner.particles_count + (_e); ++_var)

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

static inline void mark_particle_dead(Particle *p)
{
    *((uint16_t*)p) = 0xFFA5;
}

static inline bool is_particle_dead(Particle *p)
{
    return *((uint16_t*)p) == 0xFFA5;
}

#define FLOAT_IS_NEAR_ZERO(f) (fabsf(f) < 0.1f)

static void check_particle_life(ParticleSpawner &s, ParticleSystem *sys)
{
    particle_id died_count = 0;

    ITER_PARTICLES(i, s) {
        Particle &p = sys->particles[i];

        if(s.flags & SF_DieIfOffscreen &&
           p.pos.x > 340 || p.pos.x < -20 || p.pos.y < -20 || p.pos.y > 220
        ) {
            //TODO: this should maybe be related to the size of the particle
            mark_particle_dead(&p);
            ++died_count;
        }

        if((s.flags & SF_DieWhenStopped) &&
           FLOAT_IS_NEAR_ZERO(p.vel.x) &&
           FLOAT_IS_NEAR_ZERO(p.vel.y)
        ) {
            mark_particle_dead(&p);
            ++died_count;
        }
    }

    if(died_count > 0) {
        printf("\n");
        ITER_PARTICLES(i, s) {
            printf("%-01d ", is_particle_dead(&sys->particles[i]));
        }
        printf("\n");

        ITER_PARTICLES_OFFSET(i, s, 0, -1) {
            if(is_particle_dead(&sys->particles[i])) {
                sys->particles[i] = sys->particles[i + 1];
                mark_particle_dead(&sys->particles[i + 1]);


            }
        }

        printf("died_count: %d count: %d\n", died_count, s.particles_count);

        s.particles_count -= died_count;
    }
}

static inline void sim_particle(Particle &p, const ParticleSpawner &spawner)
{
    p.pos += p.vel;
    p.vel *= spawner.props.inertia;
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

        // simulate
        ITER_PARTICLES(j, s) {
            sim_particle(sys->particles[j], s);
        }

        // handle making particles disappear
        // TODO: particles should always be able to die I think
        const bool can_die = (s.flags & SF_DieWhenStopped) | (s.flags & SF_DieIfOffscreen);
        if(can_die) {
            check_particle_life(s, sys);
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
