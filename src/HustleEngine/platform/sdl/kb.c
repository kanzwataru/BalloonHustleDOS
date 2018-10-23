#include "platform/kb.h"
#include <SDL/SDL.h>

static bool should_quit = false;

void keyboard_per_frame_update(void) 
{
    SDL_Event event;
    
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                should_quit = true;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == KEY_F4 && event.key.keysym.mod & KMOD_LALT)
                    should_quit = true;
                
                break;
        }
    }
}

bool keyboard_os_quit_event(void)
{
    return should_quit;
}

bool keyboard_poll_keypress(Keypress *kp)
{
    return false;
}
