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

main: main.o
	$(COMPILER) main.o -o main $(COMP_FLAGS)

main.o: math.o entity.o item.o map.o
	$(COMPILER) src/main.cpp math.o entity.o -o main.o $(COMP_FLAGS)

math.o:
	$(COMPILER) math/random.cpp math/Vector2.cpp -o math.o $(COMP_FLAGS)

entity.o:
	$(COMPILER) src/entity/entity.cpp src/entity/player.cpp src/entity/playerstatefunc.cpp src/entity/slime.cpp -o entity.o $(COMP_FLAGS)

item.o:
	$(COMPILER) src/item/item.cpp -o item.o $(COMP_FLAGS)

map.o:
	$(COMPILER) sre/item/map.cpp -o map.o $(COMP_FLAGS)