#pragma once
#include "../commonincludes.h"
#include <vector>
#include "stats.h"

class Player;
class Entity;
struct Map;

class Entitybase {
public:
	s32 id;

	u8 dead;
	u8 hit;

	v2f pos;
	f32 forwardVel;
	f32 deltaTime;

	v2f leftAxis;

	v2f size;

	Stats stats;

	Texture2D *texture;
	v2f animRect;
	s32 animTimer;
	s32 xp;
public:
	virtual void Update(Player *p, std::vector<Entity *> &e, Map *map) = 0;
	virtual void Draw() = 0;
	virtual bool checkCollision(Entity *e) = 0;
	virtual void collide(std::vector<Entity *> &e) = 0;
	virtual void receiveDamage(Player *p) = 0;
};

class Entity : public Entitybase {
public:
	inline Entity(v2f pos, Stats stats, v2f size, s32 id) {
		this->pos = pos;
		this->stats = stats;
		this->size = size;
		this->id = id;
		this->dead = 0;
		this->animRect = { 0, 0 };
		this->animTimer = 6;
	};

	virtual void Draw();
	
	void Update(Player *p, std::vector<Entity *> &e, Map *map);
	void collide(std::vector<Entity *> &e);
	void collideMap(Map *map, bool colHole);
	void receiveDamage(Player *p);
	virtual ~Entity();

	bool checkCollision(Entity *e);
	Rectangle Rect();
};

f32 getAngle(f32 x, f32 y);