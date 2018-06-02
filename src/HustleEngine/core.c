#include "src/hustle~1/core.h"
#include <dos.h>

#define INPUT_STATUS_0 0x3da  /* Used for querying Vblank */

void engine_start(CoreData cd)
{
    /* Game Loop */
    while(cd.input_handler()) {
        cd.update_callback();

        /* v retrace */
        while(inportb(INPUT_STATUS_0) & 8) {;}
        while(!(inportb(INPUT_STATUS_0) & 8)) {;}

        cd.render_callback();
    }

    cd.exit_handler();
}
