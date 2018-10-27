#
# WreckingBalloon make file
#

GAME_NAME		:= balloon
GAME_INCLUDE	:= src/WreckingBalloon src
GAME_OBJS		:= src/main.o src/WreckingBalloon/title.o src/WreckingBalloon/wb.o src/WreckingBalloon/resource.o src/WreckingBalloon/cactoon.o src/WreckingBalloon/balloon.o
DOS_ENV_DIR		:= "C:\\DEV\\BALLOON"
DOS_ENGINE_DIR  := "$(DOS_ENV_DIR)\\ENGINE"
ENGINE_DIR		:= engine
BUILD_DIR       := build

# *** include the engine's makefile *** #
include $(ENGINE_DIR)/hustle-build.mak  #
# ************************************* #

prebuild:
	# things that should happen before the compilation
	echo "** Prebuild **"

postbuild: mainbuild
	# things that should happen after the compilation
	echo "** Postbuild **"

cleanhook:
	# add to the `make clean` target
	echo "** Clean **"
