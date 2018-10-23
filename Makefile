COMMON_HEADERS     = src/HustleEngine src/WreckingBalloon src
COMMON_GAME_OBJS   = balloon.o cactoon.o resource.o wb.o
COMMON_ENGINE_OBJS = engine/core.o engine/event.o engine/render.o platform/filesys.o

COMMON_OBJS 	   = $(addprefix src/HustleEngine/,$(COMMON_ENGINE_OBJS))
COMMON_OBJS       += $(addprefix src/WreckingBalloon/,$(COMMON_GAME_OBJS))
COMMON_OBJS       += src/main.o src/test.o src/simplet.o

ifeq ($(TARGET_PLATFORM), unix)
include unix-makefile
else ifeq ($(TARGET_PLATFORM), dos)
include dos-makefile
else
$(error Unsupported platform or TARGET_PLATFORM not specified)
endif

clean:
	rm -f $(COMMON_OBJS)
	rm -f *.EXE
	rm -f *.OBJ
	rm -f *.DSK
	rm -f *.ERR
	rm -f *.MAP
	rm -f *.LK1
	rm -f *.SWP
	rm -f *.CFG
	find . -type f -name '._*' -delete

