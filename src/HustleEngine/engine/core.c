#include "engine/core.h"
#include "common/platform.h"
#include "engine/event.h"
#include "platform/video.h"

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
        video_wait_vsync();

        if(--skip_counter> 0)
            goto VSync;

        cd.render_callback();
        skip_counter = frame_skip;
    }

    cd.exit_handler();
}
