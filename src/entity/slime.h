#pragma once
#include "entity.h"

class Player;

class Slime : public Entity {
private:
	u8 collison;
public:

	inline Slime(v2f pos, s32 id) : Entity(pos, {10, 10, 0, 0, 1, 0, 1}, {32, 32}, id) {};
	
	virtual void Update(Player *p, std::vector<Entity *> e, Map *map);
	bool checkCollision(Entity *e);
};