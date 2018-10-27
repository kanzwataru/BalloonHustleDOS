#include "title.h"
#include "resource.h"

#include "engine/core.h"
#include "engine/render.h"
#include "engine/event.h"
#include "platform/kb.h"
#include "platform/filesys.h"
#include "platform/video.h"

#define TITLE_SCREEN    "RES/TITLE.BMP"

static RenderData rd;
static buffer_t  *title_palette;
static buffer_t  *black_palette;
static buffer_t  *title_image;
static float      fade = 0.0;

static bool input(void)
{
    keyboard_per_frame_update();
    if(keyboard_os_quit_event() || keyboard_keys[KEY_ESC])
        return false;

    if(keyboard_keys[KEY_ENTER] || keyboard_keys[KEY_SPACE])
        engine_release();

    return true;
}

static void update(void)
{
    if(fade < 1.0f)
        fade += 0.01f;
    
    palette_fade(black_palette, title_palette, fade);
}

static void render(void)
{
    
}

static void render_flip(void)
{
    finish_frame(&rd);
}

static void quit(void)
{
    
}

void title_start(void)
{
    CoreData cd;
    
    cd.update_callback = &update;
    cd.render_callback = &render;
    cd.flip_callback = &render_flip;
    cd.input_handler = &input;
    cd.exit_handler = &quit;
    cd.frame_skip = 0;

    black_palette = create_palette();
    _fmemset(black_palette, 0, PALETTE_SIZE);

    title_palette = load_bmp_palette(TITLE_SCREEN);
    init_renderer(&rd, 1, title_palette);
    
    FILL_BUFFER(rd.screen, 0);
    
    title_image = load_bmp_image(TITLE_SCREEN);
    _fmemcpy(rd.screen, title_image, SCREEN_SIZE);
    
    engine_start(cd);
    
    destroy_image(&title_palette);
    destroy_image(&title_image);
    destroy_image(&black_palette);
    destroy_renderdata(&rd);
}
