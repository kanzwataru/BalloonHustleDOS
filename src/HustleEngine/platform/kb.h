#ifndef KB_DOS_H
#define KB_DOS_H

#include "common/platform.h"

#ifdef PLATFORM_DOS
	#include "platform/dos/kb_dos.h"
#endif
#ifdef PLATFORM_SDL
	#include "platform/sdl/kb_sdl.h"
#endif

#endif /* KB_DOS_H */
