#pragma once
#include "../../math/Vector2.h"
#include <vector>


struct Tile {
	v2i pos;
	u32 type;
};

struct Map {
	std::vector<Tile> tiles;
	v2i size;
	s32 tileSize;

	Map(v2i size);
	~Map();

	void Draw();

	v2i getTileCords(const v2f &pos);

	Tile getTile(const v2i &pos);
	Rectangle rect(const v2i &pos);
};