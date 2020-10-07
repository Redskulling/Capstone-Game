#pragma once
#include "entity.h"

class Player;

class Slime : public Entity {
private:
	u8 collison;
public:

	inline Slime(v2f pos, s32 id) : Entity(pos, {1, 1, 0, 0, 1, 0, 1}, {32, 32}, id) {};
	
	virtual void Update(Player *p, std::vector<Entity *> e);
	bool checkCollision(Entity *e);
};