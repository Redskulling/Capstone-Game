#include "entity.h"
#include "player.h"
#include "../map/map.h"
#include <stdio.h>
#include <cmath>

Entity::~Entity() {}

void Entity::Draw() {
	if (!this->dead)
		DrawRectangle(this->pos.x, this->pos.y, this->size.x, this->size.y, RED);
}

void Entity::Update(Player *p, std::vector<Entity *> &e, Map *map) {
	fprintf(stderr, "Called Update on Entity\n");
}

bool Entity::checkCollision(Entity *e) {
	fprintf(stderr, "Called Update on Entity\n");
	return -1;
}

Rectangle Entity::Rect() {
	return {this->pos.x, this->pos.y, this->size.x, this->size.y};
}

void Entity::receiveDamage(Player *p) {
	if (this->dead) return;
	if (!this->hit)
		if (CheckCollisionCircleRec(p->hitbox.centre, p->hitbox.radius, this->Rect()))
			this->stats.hp -= p->stats.str;

		this->hit = 1;

		if (this->stats.hp <= 0){
			this->stats.hp = 0;
			this->dead = 1;
			f32 result = randomFloat(0.0f, 100.0f);

			if (result >= 50.0f && result <= 75.0f)
				p->itemDrops.push_back(new Item("HEAL", 1, this->pos, p->itemDrops.size(), p->itemDrops[0]->texture));
			else if (result > 75.0f && result <= 99.0f)
				p->itemDrops.push_back(new Item("STAM", 1, this->pos, p->itemDrops.size(), p->itemDrops[0]->texture));
			else if (result > 99.0f && result <= 99.5f)
				p->itemDrops.push_back(new Item("ATT", 1, this->pos, p->itemDrops.size(), p->itemDrops[0]->texture));
			// else if (result > 99.5f && result <= 100.0f)
				// p->itemDrops.push_back(new Item("DEF", 1, this->pos, p->itemDrops.size(), p->itemDrops[0]->texture));
	}

	if (this->hit)
		if (!CheckCollisionCircleRec(p->hitbox.centre, p->hitbox.radius, this->Rect()))
			this->hit = 0;
}

f32 getAngle(f32 x, f32 y) {
	if (x == 0 && y == 0)
		return -1.0f;
	f32 angle = atan2(y, x);
	angle = (angle / PI) * 180.0f;
	angle += 45.0f;
	if (angle <= 0.0f)
		angle += 360.0f;

	return angle;
}

u8 getCollideType(bool &col, u8 colType, bool colHole) {
	if (colType & 1)
		return col &= 1;
	if (colType & (2 & (colHole << 1)))
		return col &= 1;
	return col = 0;
}

void Entity::collideMap(Map *map, bool colHole = 1) {
	u8 numCol = 0;
	if (this->dead)
		return;

	if ((map->getTilePos((this->pos)).type == 0x80000000) && 
		((map->getTile((v2u8)((this->pos + this->size) / (f32) map->tileSize)).type == 0x80000000)))
		return;

	bool col;
	s32 numCollisions = 0;

	while (numCollisions < 3) {
		Rectangle mapRect = map->Rect(this->pos);

		col = CheckCollisionRecs(mapRect, this->Rect());
		getCollideType(col, map->getTilePos(this->pos).type, colHole);

		if (!col) {
			mapRect = map->Rect(this->pos + this->size);
			col = CheckCollisionRecs(mapRect, this->Rect());
			getCollideType(col, map->getTilePos(this->pos + this->size).type, colHole);
		}

		if (!col) {
			mapRect = map->Rect(this->pos + this->size * v2f{1, 0});
			col = CheckCollisionRecs(mapRect, this->Rect());
			getCollideType(col, map->getTilePos(this->pos + this->size * v2f{1, 0}).type, colHole);
		}

		if (!col) {
			mapRect = map->Rect(this->pos + this->size * v2f{0, 1});
			col = CheckCollisionRecs(mapRect, this->Rect());
			getCollideType(col, map->getTilePos(this->pos + this->size * v2f{0, 1}).type, colHole);
		}

		if (!col)
			break;

		v2f playerTmpAngle = v2f((mapRect.x + map->tileSize / 2), (mapRect.y + map->tileSize / 2)) - (this->pos + this->size / 2.0f);
		f32 angle = getAngle(playerTmpAngle.x, playerTmpAngle.y);
		if (angle < 0)
			break;

		if (0.0f < angle && angle <= 90.0f) {
			this->pos.x = mapRect.x - this->size.x - 0.5f;
			this->leftAxis.x = 0.0f;
			numCollisions++;
			continue;
		} else if (90.0f < angle && angle <= 180.0f) {
			this->pos.y = mapRect.y - this->size.y - 0.5f;
			this->leftAxis.y = 0.0f;
			numCollisions++;
			continue;
		} else if (180.0f < angle && angle <= 270.0f) {
			this->pos.x = mapRect.x + map->tileSize + 0.5f;
			this->leftAxis.x = 0.0f;
			numCollisions++;
			continue;
		} else if (270.0f < angle && angle <= 360.0f) {
			this->pos.y = mapRect.y + map->tileSize + 0.5f;
			this->leftAxis.y = 0.0f;
			numCollisions++;
			continue;
		}
	}
}

void Entity::collide(std::vector<Entity *> &e) {

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

f32 calcDamage(Entity *attacker, Entity *defender) {
	f32 result = std::pow(1.029186009, -defender->stats.def);
	return result * attacker->stats.str;
}