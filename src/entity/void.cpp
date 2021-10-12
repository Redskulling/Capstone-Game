#include "void.h"
#include "player.h"
#include <cmath>

void Void::Update(Player *p, std::vector<Entity *> &e, Map *map) {
	if (this->dead)
		return;
	this->leftAxis = (p->pos - this->pos).norm();  // Move Slime towards player
	this->leftAxis *= (f32) this->stats.spd * 100.0f * this->deltaTime;  // Set Speed

	this->collide(e);
	this->receiveDamage(p);
}

bool Void::checkCollision(Entity *e) {
	return (CheckCollisionRecs(this->Rect(), e->Rect()));
}

void Void::Draw() {
	if (this->dead) return;

	if (++this->animTimer > GetFPS() / 8) {
		if (++this->animRect.x >= 6)
			this->animRect.x = 0;
		this->animTimer = 0;
	}

	if (this->leftAxis.x < 0)
		this->texture->width < 0 ? this->texture->width : this->texture->width *= -1;
	else if (this->leftAxis.x > 0)
		this->texture->width > 0 ? this->texture->width : this->texture->width *= -1;

	DrawTextureRec(*this->texture, { this->animRect.x * 32.0f, this->animRect.y * 32.0f, 32, 32 }, {this->pos.x - 9, this->pos.y - 16}, RED);
}

void Void::receiveDamage(Player *p) {
	if (this->dead) return;
	if (!this->hit)
		if (CheckCollisionCircleRec(p->hitbox.centre, p->hitbox.radius, this->Rect()))
			this->stats.hp -= p->stats.str;

		this->hit = 1;

		if (this->stats.hp <= 0){
			this->collideMap(p->map, 1);
			this->stats.hp = 0;
			this->dead = 1;
			p->itemDrops.push_back(new Item("ATT", 1, this->pos, p->itemDrops.size(), p->itemDrops[0]->texture));
	}

	if (this->hit)
		if (!CheckCollisionCircleRec(p->hitbox.centre, p->hitbox.radius, this->Rect()))
			this->hit = 0;
}