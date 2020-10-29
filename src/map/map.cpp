#include "map.h"
#include "../../math/random.h"

Map::Map(f32 tileSize, v2i size) : tileSize(tileSize), size(size) {
	this->tiles.reserve(size.x * size.y);
	for (s32 x = 0; x < size.x; x++) {
		for (s32 y = 0; y < size.y; y++) {
			this->tiles[x + size.x * y].type = randomInt(0, 2);
			this->tiles[x + size.x * y].pos = {x, y};
		}
	}
}

Tile::Tile(v2i pos, s32 type) : pos(pos), type(type) {};

Tile Map::getTile(v2i pos) {
	u32 index = pos.x + this->size.x * pos.y;
	if ((pos.x >= this->size.x || pos.x < 0) || (pos.y >= this->size.y || pos.y < 0))
		return {{0x8000000, 0x8000000}, 0x8000000};
	return this->tiles[index];
}

Tile Map::getTilePos(v2f pos) {
	v2i newPos = (v2i) (pos / this->tileSize);
	return this->getTile(newPos);
};

void Map::Draw() {
	for (int i = 0; i < this->size.x * this->size.y; i++) {
		if (this->tiles[i].type & 1)
			DrawRectangle(this->tiles[i].pos.x * this->tileSize, this->tiles[i].pos.y * this->tileSize, this->tileSize, this->tileSize, PURPLE);
	}
}

Rectangle Map::Rect(v2f pos) {
	Tile tile = this->getTilePos(pos);
	return {(f32) tile.pos.x * this->tileSize, (f32) tile.pos.y * this->tileSize, this->tileSize, this->tileSize};
}