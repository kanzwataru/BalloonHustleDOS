/*
 * Particle system, depends only on HustleEngine and not on BalloonHustle
 */
#ifndef BH_PARTICLES_H
#define BH_PARTICLES_H

#include "vec2d.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "common/platform.h"
#include "common/mathlib.h"

/* define settings to customize */
#ifndef PRT_MAX_PARTICLES
#define PRT_MAX_PARTICLES 128
#endif

typedef byte particle_id;

enum SpawnerFlags {
    SF_Active = (1 << 0),
};

struct ParticleProps {
    float    follow;
    uint16_t rate;
    uint16_t decay;
};

struct ParticleSpawner {
    struct ParticleProps props;
    byte  flags;
    Point prev_pos;
    Point pos;
    byte  palette_start;
    byte  palette_end;

    // internal, please initialize to zero
    particle_id particles_start;
    particle_id particles_count;
    uint16_t spawn_counter;
};

struct Particle {
    struct V2 pos;
    struct V2 vel;
};

struct ParticleSystem {
    struct Particle particles[PRT_MAX_PARTICLES];
};

void PRT_update(struct ParticleSpawner *spawners, uint16_t count, struct ParticleSystem *system);
void PRT_draw(struct ParticleSpawner *spawners, uint16_t count, struct ParticleSystem *system);

#ifdef __cplusplus
}
#endif  /* c++ */
#endif  /* BH_PARTICLES_H */
