COMMON_GAME_OBJS   = balloon.o cactoon.o resource.o wb.o
COMMON_ENGINE_OBJS = engine/core.o engine/event.o engine/render.o platform/filesys.o platform/sdl/kb.o platform/sdl/video.o

COMMON_OBJS 	   = $(addprefix src/HustleEngine/,$(COMMON_ENGINE_OBJS))
COMMON_OBJS       += $(addprefix src/WreckingBalloon/,$(COMMON_GAME_OBJS))
COMMON_HEADERS     = src/HustleEngine src/WreckingBalloon

ifeq ($(TARGET_PLATFORM), unix)
include unix-makefile
else ifeq ($(TARGET_PLATFORM), dos)
include dos-makefile
else
$(error Unsupported platform or TARGET_PLATFORM not specified)
endif

clean:
	rm $(COMMON_OBJS)
	rm -f *.EXE
	rm -f *.OBJ
	rm -f *.DSK
	rm -f *.ERR
	rm -f *.MAP
	rm -f *.LK1
	rm -f *.SWP
	rm -f *.CFG
	find . -type f -name '._*' -delete

