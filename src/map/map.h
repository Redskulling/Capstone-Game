#pragma once
#include "../../math/Vector2.h"
#include "../types.h"
#include <vector>
#include <raylib.h>

struct Tile {
	v2i pos;
	s32 type;

	Tile(v2i pos, s32 type);
};

struct Map {
	std::vector<Tile> tiles;
	f32 tileSize;
	v2i size;

	Map(f32 tileSize, v2i size);

	Tile getTile(v2i pos);
	Tile getTilePos(v2f pos);

	void Draw();
	Rectangle Rect(v2f pos);
};