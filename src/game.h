#ifndef BH_GAME_H
#define BH_GAME_H

#include "assets_main.gen.h"
#include "consts.h"
#include "components.h"
#include "statics.h"

enum EntitySlots {
	EID_PlayerBalloon = 0,
	EID_PlayerCactus  = 1,
	EID_Enemies		  = 2,
	EID_EnemiesEnd	  = 16,
	EID_Bullets		  = 17,
	EID_BulletsEnd	  = 48,

	EID_End
	#define ENTITY_MAX EID_End
};

enum EGameState {
	GAME_STATE_BEGIN  	= 0,
	GAME_STATE_PLAYING 	= 1
};

#define DEF_COMP_ARRAY(type, name) \
	struct type name[ENTITY_MAX]

struct GameData {
    struct Game *game;

    uint16_t game_state;
    byte palette[255];

	byte entity_roster[ENTITY_MAX];
    X_ALL_COMPS(DEF_COMP_ARRAY);

	struct ParticleSystem particle_sys;

    Rect clouds[CLOUD_MAX];
    float fade_percent;

	struct PakMain pak;
};

extern struct GameData *g;

// TODO: better place for these
void create_balloon_cactus(entity_id id, entity_id cactus_id, bool is_player, bool reserve);
void create_bullet(entity_id id, const Point *pos, const FLPoint *dir);

#endif
