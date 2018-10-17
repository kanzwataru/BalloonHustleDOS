#include "engine/core.h"
#include "common/platform.h"
#include "engine/event.h"
#include <dos.h>

#define INPUT_STATUS_0 0x3da  /* Used for querying Vblank */
static int skip_counter = 0;
static int frame_skip;

void engine_start(CoreData cd)
{
    frame_skip = cd.frame_skip + 1;
    skip_counter = frame_skip;

    /* Game Loop */
    while(cd.input_handler()) 
    {
        cd.update_callback();
        event_update();

        VSync:
        while(inportb(INPUT_STATUS_0) & 8) {;}
        while(!(inportb(INPUT_STATUS_0) & 8)) {;}

        if(--skip_counter> 0)
            goto VSync;

        cd.render_callback();
        skip_counter = frame_skip;
    }

    cd.exit_handler();
}
