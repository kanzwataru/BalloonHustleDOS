#include "src/hustle~1/render.h"
#include <malloc.h>
#include <dos.h>

void init_animation(Animation *anim, byte frame_count, uint w, uint h)
{
    int i;
    anim->frame_count = frame_count;
    anim->w = w;
    anim->h = h;

    anim->frames = malloc(frame_count * sizeof(buffer_t *));
    for(i = 0; i < frame_count; ++i) {
        anim->frames[i] = farmalloc(w * h);
    }
}

void destroy_animation(Animation *anim)
{
    int i;

    for(i = 0; i < anim->frame_count; ++i) {
        free(anim->frames[i]);
    }

    free(anim->frames);
    anim->frames = NULL;
}

void load_animation(Animation *anim, const char *file)
{
    //TODO
}
