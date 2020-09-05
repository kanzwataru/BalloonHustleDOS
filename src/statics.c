#include "assets_main.gen.h"
#include "statics.h"
#include <stddef.h>

const struct BalloonAnimTable s_player_balloon_anim = {
    offsetof(struct PakMain, balloon_idle),
    offsetof(struct PakMain, balloon_pop)
};

const struct CactusAnimTable  s_player_cactus_anim = {
    offsetof(struct PakMain, cac_idle),
    offsetof(struct PakMain, cac_fall)
};

const struct BalloonAnimTable s_enemy_balloon_anim = {
    offsetof(struct PakMain, enemybl_idle),
    offsetof(struct PakMain, enemybl_pop)
};

const struct CactusAnimTable  s_enemy_cactus_anim = {
    offsetof(struct PakMain, enemycac_idle),
    offsetof(struct PakMain, enemycac_fall)
};
