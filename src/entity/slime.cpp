#include "slime.h"
#include "player.h"
#include <cmath>

void Slime::Update(Player *p, std::vector<Entity *> e, Map *map) {
	if (this->dead)
		return;
	this->leftAxis = (p->pos - this->pos).norm();  // Move Slime towards player
	this->leftAxis *= (f32) this->stats.spd * 100.0f * this->deltaTime;  // Set Speed

	this->collide(e);
	this->receiveDamage(p);
}

bool Slime::checkCollision(Entity *e) {
	return (CheckCollisionRecs(this->Rect(), e->Rect()));
}

