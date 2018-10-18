#include "simpletest.h"
#include "common/platform.h"
#include "engine/render.h"
#include "engine/core.h"
#include "platform/kb.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static RenderData rd;

static bool input(void)
{
    keyboard_per_frame_update();
    if(keyboard_os_quit_event())
        return false;
    
    return true;
}

static void update(void)
{
    srand(time(NULL));
    printf("%d\n", rand());
}

static void render(void)
{
    int i;
    start_frame(&rd);
    
    for(i = 0; i < 320 * 200; ++i) {
        rd.screen[i] = rand();
    }
    
    finish_frame(&rd);
}

static void quit(void)
{
    quit_renderer(&rd);
}

void simpletest_start(void)
{
    int i;
    byte palette[256];
    CoreData cd;
    cd.update_callback = update;
    cd.render_callback = render;
    cd.input_handler   = input;
    cd.exit_handler    = quit;
    cd.frame_skip      = 0;
    
    srand(time(NULL));
    for(i = 0; i < 256; ++i) {
        palette[i] = rand();
    }
    
    rd.flags |= RENDER_DOUBLE_BUFFER;
    if(!init_renderer(&rd, 0, &palette[0]))
        PANIC("Could not initialize renderer");
    
    while(input()) {
        update();
        render();
    }
    //engine_start(cd);
}