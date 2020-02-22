#ifndef BH_STATICS_H
#define BH_STATICS_H

struct BalloonAnimTable {
    unsigned int idle_anim;
    unsigned int pop_anim;
};

struct CactusAnimTable {
    unsigned int idle_anim;
    unsigned int fall_anim;
};

extern const struct BalloonAnimTable s_player_balloon_anim;
extern const struct CactusAnimTable  s_player_cactus_anim;

extern const struct BalloonAnimTable s_enemy_balloon_anim;
extern const struct CactusAnimTable  s_enemy_cactus_anim;

#endif
