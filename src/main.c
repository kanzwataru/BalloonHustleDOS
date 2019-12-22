#include "hustle.h"
#include "platform/bootstrap.h"

void init(void) {}
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
    game->init = init;
    game->input = input;
    game->update = update;
    game->render = render;
    game->quit = quit;
}

