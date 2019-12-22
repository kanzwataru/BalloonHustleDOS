#include "hustle.h"
#include "platform/bootstrap.h"
#include "assets_main.gen.h"

struct GameData {
    struct Game *game;
    byte pak[52000];
};

static struct GameData *g;

void init(void)
{
    struct PaletteAsset *pal;

    pal = asset_get(GAMEPAL, Palette, g->pak);
    renderer_set_palette(pal->data, 0, pal->col_count);
}
void input(void) {}
void update(void) {}
void render(void)
{
    renderer_clear(2);
    renderer_flip();
}
void quit(void) {}

void HANDSHAKE_FUNCTION_NAME(struct Game *game, void *memory_chunk) 
{
    g = (struct GameData *)memory_chunk;
    g->game = game;

    game->init = init;
    game->input = input;
    game->update = update;
    game->render = render;
    game->quit = quit;

    asset_load_pak(g->pak, "main.dat");
}

