#include "map.h"
#include "../../math/random.h"
#include <iostream>
#include <fstream>

Map::Map(f32 tileSize, v2u8 size, const char *name) : tileSize(tileSize), size(size) {
	this->tileSet = LoadTexture(name);
	this->tiles.reserve(size.x * size.y + 1);
	for (s32 x = 0; x < size.x; x++) {
		for (s32 y = 0; y < size.y; y++) {
			this->tiles[x + size.x * y].type = 0;
			this->tiles[x + size.x * y].pos = {x, y};
			this->tiles[x + size.x * y].tilePos = { 4, 0 };
		}
	}
	this->tiles[size.x * size.y + 1] = Tile{{0x80, 0x80}, 0x80};
}

#ifdef MAP_DEBUG
Map::Map(const char *file, const char *fileName) {
	this->tileSet = LoadTexture(fileName);
	this->ReadFromFile(file);
}
#else
Map::Map(const char *file, const char *fileName, std::vector<Entity *> &e, Texture2D &slime) {
	this->tileSet = LoadTexture(fileName);
	this->slime = &slime;
	this->ReadFromFile(file, e);
}
#endif

void Map::NewMap(std::vector<Entity *> &ent) {
	std::string mapname = "map";
	mapname += std::to_string(randomInt(0, 2));
	mapname += ".map";
	printf("%i\n", ent.size());
	for (auto &e : ent)
		if (e->id != 0) delete e;
	printf("%i\n", ent.size());
	ent.resize(1);
	printf("%i\n", ent.size());
	this->ReadFromFile(mapname.c_str(), ent);
	printf("%i\n", ent.size());
}

Map::~Map() {
	UnloadTexture(this->tileSet);
}

Tile::Tile(v2u8 pos, u8 type) : pos(pos), type(type) {
	tilePos = {0, 0};
};

Tile &Map::getTile(v2u8 pos) {
	u32 index = pos.x + this->size.x * pos.y;
	if ((pos.x >= this->size.x || pos.x < 0) || (pos.y >= this->size.y || pos.y < 0))
		return this->tiles[this->tiles.size() - 1];
	return this->tiles[index];
}

Tile &Map::getTilePos(v2f pos) {
	v2u8 newPos = (v2u8) (pos / this->tileSize);
	return this->getTile(newPos);
};

void Map::Draw(Camera2D &camera) {
	v2f top = (v2f)camera.target - (v2f)camera.offset / camera.zoom;
	if (top.x < 0)
		top.x = 0;
	if (top.y < 0)
		top.y = 0;

	v2f bottom = ((v2f) camera.target + (v2f) camera.offset / camera.zoom);
	if (bottom.x >= (f32) this->size.x * this->tileSize)
		bottom.x = (f32) this->size.x * this->tileSize - 1.0f;

	if (bottom.y >= (f32) this->size.y * this->tileSize)
		bottom.y = (f32) this->size.y * this->tileSize - 1.0f;

	v2u8 startPos = this->getTilePos(top).pos;
	v2u8 endPos = this->getTilePos(bottom).pos;

	for (u32 x = startPos.x; x <= endPos.x; x++) {
		for (u32 y = startPos.y; y <= endPos.y; y++) {
			const Tile &drawTile = this->getTile({x, y});
			Rectangle drawRec = { ((f32) drawTile.tilePos.x * 32.0f), ((f32) drawTile.tilePos.y * 32.0f), 32.0f, 32.0f };

			DrawTextureRec(this->tileSet, drawRec, ((v2f) drawTile.pos * 32.0f), WHITE);
			#ifdef MAP_DEBUG
			if (drawTile.type & 0x01)
				DrawRectangle(drawTile.pos.x * 32.0f, drawTile.pos.y * 32.0f , 5, 5, PURPLE);
			if (drawTile.type & 0x02)
				DrawRectangle(drawTile.pos.x * 32.0f + 5, drawTile.pos.y * 32.0f, 5, 5, ORANGE);
			if (drawTile.type & 0x04)
				DrawRectangle(drawTile.pos.x * 32.0f + 10, drawTile.pos.y * 32.0f, 5, 5, BLUE);
			if (drawTile.type & 0x08)
				DrawRectangle(drawTile.pos.x * 32.0f + 15, drawTile.pos.y * 32.0f, 5, 5, WHITE);
			if (drawTile.type & 0x10)
				DrawRectangle(drawTile.pos.x * 32.0f + 20, drawTile.pos.y * 32.0f, 5, 5, GREEN);
			if (drawTile.type & 0x20)
				DrawRectangle(drawTile.pos.x * 32.0f, drawTile.pos.y * 32.0f + 5, 5, 5, GRAY);
			if (drawTile.type & 0x40)
				DrawRectangle(drawTile.pos.x * 32.0f + 5, drawTile.pos.y * 32.0f + 5, 5, 5, DARKBLUE);

			#endif
		}
	}
}

Rectangle Map::Rect(v2f pos) {
	Tile tile = this->getTilePos(pos);
	return {(f32) tile.pos.x * this->tileSize, (f32) tile.pos.y * this->tileSize, this->tileSize, this->tileSize};
}

void Map::WriteToFile(const char *file) {
	puts("Saving...");
	std::ofstream myfile;
	myfile.open(file, std::ios::out | std::ios::binary);
	if (myfile.is_open()) {
		myfile.write(reinterpret_cast<char *>(&this->size), sizeof(v2u8));
		myfile.write(reinterpret_cast<char *>(&this->tileSize), sizeof(f32));
		myfile.write(reinterpret_cast<char *>(&this->tiles[0]), this->tiles.capacity() * sizeof(Tile));
		myfile.close();
		puts("Saved!");
	} else {
		puts("Failed To Save Map :(");
	}
}

void Map::ReadFromFile(const char *file 
#ifndef MAP_DEBUG
, std::vector<Entity *> &ent) {
#else
) {
#endif
	puts("Loading...");
	std::ifstream myfile;
	myfile.open(file, std::ios::in | std::ios::binary);
	if (myfile.is_open()) {
		myfile.read(reinterpret_cast<char *>(&this->size), sizeof(v2u8));
		myfile.read(reinterpret_cast<char *>(&this->tileSize), sizeof(f32));

		this->tiles.clear();
		this->tiles.reserve(this->size.x * this->size.y);
		for (int i = 0; i < this->size.x * this->size.y; i++) {
			Tile tmpTile = Tile{{ 0, 0 }, 0 };
			myfile.read(reinterpret_cast<char *>(&tmpTile), sizeof(Tile));
			this->tiles.push_back(tmpTile);
			#ifndef MAP_DEBUG
			if (tmpTile.type & 0x08)
				ent[0]->pos = (v2f) tmpTile.pos * 32.0f;
			if (tmpTile.type & 0x10) {
				ent.emplace_back(new Slime((v2f) tmpTile.pos * 32.0f, ent.size()));
			}
			#endif
		}
		for (auto &a : ent)
			if (a->id != 0) a->texture = slime;
		puts("Loaded!");
	} else {
		puts("Failed To Load Map :{");
	}
}