#include "assets_main.gen.h"
#include "statics.h"

const struct BalloonAnimTable s_player_balloon_anim = {
    ASSET_BALLOON_IDLE,
    ASSET_BALLOON_POP
};

const struct CactusAnimTable  s_player_cactus_anim = {
    ASSET_CAC_IDLE,
    ASSET_CAC_FALL
};

const struct BalloonAnimTable s_enemy_balloon_anim = {
    ASSET_ENEMYBL_IDLE,
    ASSET_ENEMYBL_POP
};

const struct CactusAnimTable  s_enemy_cactus_anim = {
    ASSET_ENEMYCAC_IDLE,
    ASSET_ENEMYCAC_FALL
};
