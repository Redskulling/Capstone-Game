#include "entity.h"
#include <stdio.h>

Entitybase::~Entitybase() {}

void Entity::Draw() {
	DrawRectangle(this->pos.x, this->pos.y, this->size.x, this->size.y, RED);
}

void Entity::Update(Player *p, std::vector<Entity *> e) {
	fprintf(stderr, "Called Update on Entity");
}

bool Entity::checkCollision(Entity *e) {
	fprintf(stderr, "Called Update on Entity");
	return -1;
}

Rectangle Entity::rect() {
	return {this->pos.x, this->pos.y, this->size.x, this->size.y};
}