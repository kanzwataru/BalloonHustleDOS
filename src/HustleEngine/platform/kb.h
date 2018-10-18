#ifndef KB_H
#define KB_H

#include "common/platform.h"

#ifdef PLATFORM_DOS
	#include "platform/dos/kb_dos.h"
#endif
#ifdef PLATFORM_SDL
	#include "platform/sdl/kb_sdl.h"
#endif

typedef struct {
	Key  key; /* Key type is defined per platform */
	bool released;
} Keypress;

/*
 * Call before checking keys
*/
void keyboard_per_frame_update(void);

/*
 * Whether an OS quit event prompts the application to quit
 *
 * (Not used in DOS, mostly for SDL)
*/
bool keyboard_os_quit_event(void);

/*
 * Get a keyboard press event, poll until there is no more
 *
 * Returns false when there are no more keys to get
*/
bool keyboard_poll_keypress(Keypress *kp);

#endif /* KB_H */
