#include "map.h"
#include "../../math/random.h"
#include <raylib.h>

Map::Map(v2i size) : size(size) {
	this->tileSize = 50;
	this->tiles.reserve(size.x * size.y);
	for (int i = 0; i < this->size.x; i++) {
		for (int j = 0; j < this->size.y; j++) {
			tiles.push_back({{i, j}, (u32) randomInt(0, 2)});
		}
	}
}

Map::~Map() {};

v2i Map::getTileCords(const v2f &pos) {
	return (( (v2i) pos) + this->size) / this->tileSize;
}

Tile Map::getTile(const v2i &pos) {
	if (((this->size.x > pos.x) || (this->tileSize > pos.y)) || ((pos.x > 0) || (pos.y > 0)))
		return this->tiles[pos.x + this->size.x * pos.y];
	return {{-1, -1}, 0x80000000};
}

void Map::Draw() {
	for (s32 i = 0; i < this->size.x; i++) {
		for (s32 j = 0; j < this->size.y; j++) {
			if (this->getTile({i, j}).type & 0x1)
				DrawRectangle(this->getTile({0, 0}).pos.x + (i * this->tileSize), this->getTile({0, 0}).pos.y + (j * this->tileSize),
					this->tileSize, this->tileSize, PURPLE);
		}
	}
}

Rectangle Map::rect(const v2i &pos) {
	v2f temp = (v2f) this->getTile(pos).pos;
	Rectangle r = { 0 };
	r.x = temp.x;
	r.y = temp.y;
	r.width = this->tileSize;
	r.height = this->tileSize;
	return r;
}