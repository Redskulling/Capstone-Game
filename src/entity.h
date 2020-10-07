#pragma once
#include "commonincludes.h"
#include <vector>
#include "stats.h"

class Player;
class Entity;

class Entitybase {
public:
	s32 id;

	v2f pos;
	f32 forwardVel;
	f32 deltaTime;

	v2f leftAxis;

	v2f size;

	Stats stats;
public:
	virtual void Update(Player *p, std::vector<Entity *> e) = 0;
	virtual void Draw() = 0;
	virtual bool checkCollision(Entity *e) = 0;
	virtual ~Entitybase();
};

class Entity : public Entitybase {
public:
	inline Entity(v2f pos, Stats stats, v2f size, s32 id) {
		this->pos = pos;
		this->stats = stats;
		this->size = size;
		this->id = id;
	};
	
	void Draw();
	void Update(Player *p, std::vector<Entity *> e);
	bool checkCollision(Entity *e);
	Rectangle rect();
};