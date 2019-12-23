#ifndef BH_GAME_H
#define BH_GAME_H

#include "assets_main.gen.h"
#include "components.h"

#define ENTITY_MAX 2

struct GameData {
    struct Game *game;
    byte pak[640000];

    struct Sprite           sprites[ENTITY_MAX];
    struct RopeComp         ropes[ENTITY_MAX];
    struct TransformComp    transforms[ENTITY_MAX];
    struct BalloonComp      balloons[ENTITY_MAX];
    struct ColliderComp     colliders[ENTITY_MAX];
};

extern struct GameData *g;

#endif

