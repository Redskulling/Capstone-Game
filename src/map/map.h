#pragma once
#include "../../math/Vector2.h"
#include "../types.h"
#include <vector>
#include <raylib.h>

#ifndef MAP_DEBUG
#include "../entity/entity.h"
#include "../entity/slime.h"
#include "../entity/void.h"
#endif

struct Tile {
	v2u8 pos;
	u8 type;

	v2u8 tilePos;

	Tile(v2u8 pos, u8 type);
};

struct Map {
	std::vector<Tile> tiles;
	f32 tileSize;
	v2u8 size;
	Texture2D tileSet;
	Texture2D *slime;

	Map(f32 tileSize, v2u8 size, const char *file);
	~Map();
#ifndef MAP_DEBUG
	Map(const char *file, const char *tileSet, std::vector<Entity *> &e, Texture2D &slime);
#else
	Map(const char *file, const char *tileSet);
#endif
	Tile &getTile(v2u8 pos);
	Tile &getTilePos(v2f pos);

	void Draw(Camera2D &camera);
	Rectangle Rect(v2f pos);
	#ifndef MAP_DEBUG
	void NewMap(std::vector<Entity *>&);
	#endif
	void WriteToFile(const char *file);
	void ReadFromFile(const char *file
#ifndef MAP_DEBUG
, std::vector<Entity *> &ent);
#else
);
#endif
};