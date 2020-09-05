#ifndef BH_GAME_H
#define BH_GAME_H

#include "assets_main.gen.h"
#include "consts.h"
#include "components.h"
#include "statics.h"

enum EGameState {
	GAME_STATE_BEGIN  	= 0,
	GAME_STATE_PLAYING 	= 1
};

struct GameData {
    struct Game *game;

    uint16_t game_state;
    byte palette[255];

    struct Sprite           sprites[ENTITY_MAX];
    struct RopeComp         ropes[ENTITY_MAX];
    struct TransformComp    transforms[ENTITY_MAX];
    struct BalloonComp      balloons[ENTITY_MAX];
    struct CactusComp       cactuses[ENTITY_MAX];
	struct ShootComp        shoots[ENTITY_MAX];
    struct ColliderComp     colliders[ENTITY_MAX];
    struct AIComp           ai[ENTITY_MAX];

    Rect clouds[CLOUD_MAX];
    float fade_percent;

	struct PakMain pak;
};

extern struct GameData *g;

// TODO: better place for this
void create_balloon_cactus(entity_id id, entity_id cactus_id, bool is_player);

#endif
