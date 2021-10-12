#pragma once
#include "entity.h"
#include "../../math/random.h"

class Player;

class Void : public Entity {
private:
	u8 collison;
public:
	inline Void(v2f pos, s32 id) : Entity(pos, {15.0f, 15.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f}, {15, 15}, id) {
		this->animTimer = randomInt(0, 15);
		this->xp = 5;
	};
	
	virtual void Update(Player *p, std::vector<Entity *> &e, Map *map);
	bool checkCollision(Entity *e);
	virtual void receiveDamage(Player *p);
	virtual void Draw();
};