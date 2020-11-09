#include "player.h"
#include "../map/map.h"
#include <stdio.h>


void Player::playAnim(s32 y, f32 boost) {
	if ((this->invincibleTimer -= 1 * this->deltaTime) <= 0)
		this->animRect.y = y;
	else if ((this->invincibleTimer > 0) && (this->animTimer > GetFPS() / 16))
		this->animRect.y = 4;
	else
		this->animRect.y = y;

	if (++this->animTimer > GetFPS() / 8 / boost) {
		if (++this->animRect.x > 4)
			this->animRect.x = 0;
		this->animTimer = 0;
	}
}

void useitem(Player *player) {
	if (player->input & INPUT_ITEM_1 || player->input & INPUT_ITEM_2) {
		switch (player->inventory.useItem((player->input & INPUT_ITEM_2 ? 1 : 0))) {
			case 1:
				player->stats.hp += 5;
				if (player->stats.hp > player->stats.maxhp)
					player->stats.hp = player->stats.maxhp;
				break;
			case 2:
				player->stats.def += 1;
				break;
			case 3:
				player->stats.str += 1;
				break;
			case 4:
				player->stamina += 40;
				if (player->stamina > player->maxStamina)
					player->stamina = player->maxStamina;
				break;
		}
	}
}

void Player::stateStationary() {

	this->playAnim(3);

	useitem(this);

	this->stamina += 1.2f * this->deltaTime;
	if (this->stamina > this->maxStamina)
		this->stamina = this->maxStamina;

	if (this->leftAxis)
		return this->setState(PLAYER_STATE_MOVING);

	if (this->input & INPUT_ATTACK) {
		this->dashTimer = 0.125f;
		return this->setState(PLAYER_STATE_ATTACKING);
	}
}

void Player::stateMoving() {

	this->playAnim(0);

	useitem(this);

	this->pos += this->leftAxis * (this->forwardVel * this->deltaTime);

	this->stamina += 1.2f * this->deltaTime;
	if (this->stamina > this->maxStamina)
		this->stamina = this->maxStamina;

	this->collideMap(this->map, 1);
	this->collideItems();

	if (this->input & INPUT_DASH_PRESSED) {
		this->dashTimer = 0.125f;
		return this->setState(PLAYER_STATE_DASH);
	}
	
	if (this->input & INPUT_ATTACK) {
		this->dashTimer = 0.125f;
		return this->setState(PLAYER_STATE_ATTACKING);
	}

	if (this->input & INPUT_SPRINT_PRESSED)
		return this->setState(PLAYER_STATE_SPRINTING);

	if (!this->leftAxis)
		return this->setState(PLAYER_STATE_STATIONARY);

}

void Player::stateSprinting() {

	this->playAnim(0, 2);

	useitem(this);

	if (this->stamina <= 1.0f * this->deltaTime)
		return this->setState(PLAYER_STATE_MOVING);

	v2f move = (this->leftAxis * (this->forwardVel * this->deltaTime));

	if (move)
		this->stamina -= 1.0f * this->deltaTime;

	this->pos += move;
	this->collideMap(this->map, 1);
	this->collideItems();

	this->pos += move;
	this->collideMap(this->map, 1);
	this->collideItems();

	if (!(this->input & INPUT_SPRINT_PRESSED))
	this->setState(PLAYER_STATE_MOVING);
}

void Player::stateDash() {

	this->playAnim(1);

	if (this->stamina <= 80.0f * this->deltaTime)
		return this->setState(PLAYER_STATE_MOVING);

	this->stamina -= 80.0f * this->deltaTime;

	v2f ceilLeftAxis = {(this->leftAxis.x > 0 ? ceilf32(this->leftAxis.x) : floorf32(this->leftAxis.x)), 
		(this->leftAxis.y > 0 ? ceilf32(this->leftAxis.y) : floorf32(this->leftAxis.y)) };

	this->pos += ceilLeftAxis * 500.0f * this->deltaTime;

	this->collideMap(this->map, 0);
	this->collideItems();

	if ((this->dashTimer -= this->deltaTime) <= 0) {
		this->dashTimer = 0;
		return this->setState(PLAYER_STATE_MOVING);
	}

	if (this->input & INPUT_ATTACK) {
		this->dashTimer = 0.125f;
		return this->setState(PLAYER_STATE_ATTACKING);
	}
}

void Player::stateAttacking() {
	this->playAnim(2);
	this->hitbox.centre = (v2i) ((this->pos + v2f{10, 15}) + (this->rightAxis * this->hitbox.radius * 2.0f));
	if ((this->dashTimer -= this->deltaTime) <= 0) {
		this->dashTimer = 0;
		this->hitbox.centre = { MAX_NEG_INT, MAX_NEG_INT };
		this->setState(PLAYER_STATE_MOVING);
	}
}

void Player::runState(Map *map, std::vector<Entity *> &e) {
	this->map = map;
	if (this->state != PLAYER_STATE_DASH)
		this->collide(e);
	if (this->dead)
		return;
	
	switch (this->state) {
		case PLAYER_STATE_MOVING:     this->stateMoving();     break;
		case PLAYER_STATE_DASH:       this->stateDash();       break;
		case PLAYER_STATE_STATIONARY: this->stateStationary(); break;
		case PLAYER_STATE_ATTACKING:  this->stateAttacking();  break;
		case PLAYER_STATE_SPRINTING:  this->stateSprinting();  break;
	}
	if (this->xp >= this->nextLevel) {
		this->stats.maxhp += 1;
		this->stats.hp = this->stats.maxhp;
		this->xp -= this->nextLevel;
		this->nextLevel += 10 * (this->stats.maxhp - 10);
		this->maxStamina += 10;
		this->stamina = this->maxStamina;
	}
}
