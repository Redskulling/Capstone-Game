#pragma once
#include "entity.h"
#include "../../math/random.h"

class Player;

class Slime : public Entity {
private:
	u8 collison;
public:
	inline Slime(v2f pos, s32 id) : Entity(pos, {2, 2, 0, 0, 1, 0, 1}, {15, 15}, id) {
		this->animTimer = randomInt(0, 15);
		this->xp = 5;
	};
	
	virtual void Update(Player *p, std::vector<Entity *> &e, Map *map);
	bool checkCollision(Entity *e);
	virtual void Draw();
};