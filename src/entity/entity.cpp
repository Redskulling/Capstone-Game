#include "entity.h"
#include "player.h"
#include "../map/map.h"
#include <stdio.h>

Entitybase::~Entitybase() {}

void Entity::Draw() {
	if (!this->dead)
		DrawRectangle(this->pos.x, this->pos.y, this->size.x, this->size.y, RED);
}

void Entity::Update(Player *p, std::vector<Entity *> e, Map *map) {
	fprintf(stderr, "Called Update on Entity");
}

bool Entity::checkCollision(Entity *e) {
	fprintf(stderr, "Called Update on Entity");
	return -1;
}

Rectangle Entity::Rect() {
	return {this->pos.x, this->pos.y, this->size.x, this->size.y};
}

void Entity::receiveDamage(Player *p) {
	if (!this->hit)
		if (CheckCollisionCircleRec(p->hitbox.centre, p->hitbox.radius, this->Rect()))
			this->stats.hp -= p->stats.str;

		this->hit = 1;

		if (this->stats.hp <= 0){
			this->stats.hp = 0;
			this->dead = 1;
	}

	if (this->hit)
		if (!CheckCollisionCircleRec(p->hitbox.centre, p->hitbox.radius, this->Rect()))
			this->hit = 0;
}

f32 getAngle(f32 x, f32 y) {
	if (x == 0 && y == 0)
		return -1.0f;
	f32 angle = atan2(y, x);
	angle = (angle / M_PI) * 180.0f;
	angle += 45.0f;
	if (angle <= 0.0f)
		angle += 360.0f;

	return angle;
}

void Entity::collideMap(Map *map) {
	u8 numCol = 0;
	if (this->dead)
		return;

	if ((map->getTile(map->getTileCords(this->pos)).type == 0x80000000) && 
		((map->getTile((v2i)((this->pos + this->size) / (f32) map->tileSize)).type == 0x80000000)))
		return;


	bool col;
	s32 numCollisions = 0;

	while (numCollisions < 3) {
		f32 angle = getAngle(this->leftAxis.x, this->leftAxis.y);
		if (angle < 0)
			break;
		
		Rectangle mapRect = map->rect((v2i) (this->pos / (f32) map->tileSize));
		// mapRect = (((map->getTile(map->getTileCords(this->pos)).type & 1) ||
		// (map->getTile(map->getTileCords(this->pos + this->size)).type & 1)) ? mapRect : Rectangle{ NAN });
		col = CheckCollisionRecs(mapRect, this->Rect());

		col &= ((map->getTile(map->getTileCords(this->pos)).type & 1) ||
		        (map->getTile(map->getTileCords(this->pos + this->size)).type & 1));

		if (!col)
			break;

		if (0.0f < angle && angle <= 90.0f) {
			this->pos.x = ( mapRect.x * map->tileSize ) - this->size.x - 1.0f;
			this->leftAxis.x = 0.0f;
			numCollisions++;
			continue;
		} else if (90.0f < angle && angle <= 180.0f) {
			this->pos.y = ( mapRect.y * map->tileSize ) - this->size.y - 1.0f;
			this->leftAxis.y = 0.0f;
			numCollisions++;
			continue;
		} else if (180.0f < angle && angle <= 270.0f) {
			this->pos.x = ( mapRect.x * map->tileSize ) + map->tileSize + 1.0f;
			this->leftAxis.x = 0.0f;
			numCollisions++;
			continue;
		} else if (270.0f < angle && angle <= 360.0f) {
			this->pos.y = ( mapRect.y * map->tileSize ) + map->tileSize + 1.0f;
			this->leftAxis.y = 0.0f;
			numCollisions++;
			continue;
		}
	}
}

void Entity::collide(std::vector<Entity *> e) {

	this->pos += this->leftAxis;

	u8 numCol = 0;

	for (int i = 0; i < e.size(); i++) {
		if (e[i]->dead)
			continue;
		if (this->id == e[i]->id)
			continue;
	
		Entity *entity = e[i];

		v2f temp = (entity->pos - this->pos).norm();
		
		f32 angle = getAngle(temp.x, temp.y);

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
};