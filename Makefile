# HustleEngine game makefile template
#
# The engine has a set of makefiles for the various target platforms,
# so the compilation is all taken care of on the engine side.
#
# All that is needed is to list the game's
# header directories and object files, the rest will be taken care of
# in the engine's makefiles.
#

GAME_NAME		:= balloon
GAME_INCLUDE	:= src
GAME_SRC		:= src/main.c\
				   src/components.c\
				   src/events.c\
				   src/statics.c\
				   src/particles/particles.cpp
ENGINE_DIR		:= engine
DEFINES			:=

# *** include the engine's makefile *** #
include $(ENGINE_DIR)/hustle-build.mk  #
# ************************************* #

prebuild:
	@# things that should happen before the compilation
	@echo "** Prebuild **"

postbuild: game
	@# things that should happen after the compilation
	@echo "** Postbuild **"

cleanhook:
	@# add to the `make clean` target
	@echo "** Clean **"
