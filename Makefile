#
# WreckingBalloon make file
#

GAME_NAME		:= balloon
GAME_INCLUDE	:= src/WreckingBalloon src
GAME_SRC		:= src/main.c src/WreckingBalloon/title.c src/WreckingBalloon/wb.c src/WreckingBalloon/resource.c src/WreckingBalloon/cactoon.c src/WreckingBalloon/balloon.c
ENGINE_DIR		:= engine
BUILD_DIR       := build

# *** include the engine's makefile *** #
include $(ENGINE_DIR)/hustle-build.mak  #
# ************************************* #

prebuild:
	@# things that should happen before the compilation
	@echo "** Prebuild **"

postbuild: mainbuild
	@# things that should happen after the compilation
	@echo "** Postbuild **"
	rsync -av RES/ $(BUILD_DIR)/RES/

cleanhook:
	@# add to the `make clean` target
	@echo "** Clean **"
