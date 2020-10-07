#include "slime.h"
#include "player.h"
#include <cmath>

void Slime::Update(Player *p, std::vector<Entity *> e) {
	this->leftAxis = (p->pos - this->pos).norm();  // Move Slime towards player
	this->leftAxis *= (f32) this->stats.spd * 100.0f * this->deltaTime;  // Set Speed

	f32 angle = atan2(this->leftAxis.y, this->leftAxis.x);
	angle = (angle / M_PI) * 180.0f;
	angle += 45.0f;
	if (angle <= 0.0f)
		angle += 360.0f;

	this->pos += this->leftAxis;

	

	u8 numCol = 0;

	for (int i = 0; i < e.size(); i++) {
		if (this->id == e[i]->id)
			continue;

		Entity *entity = e[i];

		if (numCol >= 3)
			break;

		bool col = this->checkCollision(entity);
		if (0.0f < angle && angle <= 90.0f) {
			if (col) {
				this->pos.x = entity->pos.x - this->size.x - 1.0f;
				this->leftAxis.x = 0.0f;
				i = -1;
				numCol++;
				continue;
			}
		} else if (90.0f < angle && angle <= 180.0f) {
			if (col) {
				this->pos.y = entity->pos.y - this->size.y - 1.0f;
				this->leftAxis.y = 0.0f;
				i = -1;
				numCol++;
				continue;
			}
		} else if (180.0f < angle && angle <= 270.0f) {
			if (col) {
				this->pos.x = entity->pos.x + entity->size.x + 1.0f;
				this->leftAxis.x = 0.0f;
				i = -1;
				numCol++;
				continue;
			}
		} else if (270.0f < angle && angle <= 360.0f) {
			if (col) {
				this->pos.y = entity->pos.y + entity->size.y + 1.0f;
				this->leftAxis.y = 0.0f;
				i = -1;
				numCol++;
				continue;
			}
		}
	}

	DrawText(FormatText("%f", angle), (s32) this->pos.x, (s32) this->pos.y, 6, GRAY);
}

bool Slime::checkCollision(Entity *e) {
	return (CheckCollisionRecs(this->rect(), e->rect()));
}

