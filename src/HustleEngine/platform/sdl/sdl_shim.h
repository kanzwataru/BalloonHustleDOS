#ifndef SDL_SHIM_H
#define SDL_SHIM_H

#ifdef SDL2
    #include <SDL2/SDL.h>

    #define SDL_SHIM_GET_KEYARRAY()   SDL_GetKeyboardState(NULL);
#else
    #include <SDL/SDL.h>
    
    #define SDL_SHIM_GET_KEYARRAY()   SDL_GetKeyStatus(NULL);
#endif

#endif
