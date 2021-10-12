TARGET_WEB ?= 0
COMPILER ?= g++

ifeq ($(TARGET_WEB),1)
	COMPILER ?= emcc
endif

BUILD_DIR_BASE := build

OPT_FLAGS := -g

CPP_VERSION := -std=c++20

RAYLIB_FLAGS := -I -L -lraylib -lGL

COMP_FLAGS := $(OPT_FLAGS) $(CPP_VERSION) $(RAYLIB_FLAGS)

MATH:= math/random.cpp math/Vector2.cpp
ENTITY := src/entity/entity.cpp src/entity/player.cpp src/entity/playerstatefunc.cpp src/entity/slime.cpp
ITEM := src/item/item.cpp
MAP:= src/map/map.cpp

MAIN := src/main.cpp $(MATH) $(ENTITY) $(ITEM) $(MAP)
main:
	$(COMPILER) $(MAIN) -o main $(COMP_FLAGS)

