#include "player.h"
#include "../map/map.h"
#include <stdio.h>

void Player::stateStationary() {
	// puts(" ");
	if (this->leftAxis)
		return this->setState(PLAYER_STATE_MOVING);

	if (this->input & INPUT_ATTACK) {
		this->dashTimer = 0.125f;
		return this->setState(PLAYER_STATE_ATTACKING);
	}
}

void Player::stateMoving() {
	this->pos += this->leftAxis * (this->forwardVel * this->deltaTime);

	this->collideMap(this->map);
	this->collideItems();

	if (this->input & INPUT_DASH_PRESSED) {
		this->dashTimer = 0.125f;
		return this->setState(PLAYER_STATE_DASH);
	}
	
	if (this->input & INPUT_ATTACK) {
		this->dashTimer = 0.125f;
		return this->setState(PLAYER_STATE_ATTACKING);
	}

	if (!this->leftAxis)
		return this->setState(PLAYER_STATE_STATIONARY);

}

void Player::stateDash() {
	v2f ceilLeftAxis = {(this->leftAxis.x > 0 ? ceilf32(this->leftAxis.x) : floorf32(this->leftAxis.x)), 
		(this->leftAxis.y > 0 ? ceilf32(this->leftAxis.y) : floorf32(this->leftAxis.y)) };

	
	this->pos += ceilLeftAxis * 500.0f * this->deltaTime;

	this->collideMap(this->map);
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
	this->hitbox.centre = (v2i) ((this->pos + v2f{ 25.0f, 25.0f }) + (this->rightAxis * this->hitbox.radius * 2.0f));
	if ((this->dashTimer -= this->deltaTime) <= 0) {
		this->dashTimer = 0;
		this->hitbox.centre = { MAX_NEG_INT, MAX_NEG_INT };
		this->setState(PLAYER_STATE_MOVING);
	}
}

void Player::runState(Map *map) {
	this->map = map;
	switch (this->state) {
		case PLAYER_STATE_MOVING:     this->stateMoving();     break;
		case PLAYER_STATE_DASH:       this->stateDash();       break;
		case PLAYER_STATE_STATIONARY: this->stateStationary(); break;
		case PLAYER_STATE_ATTACKING:  this->stateAttacking();  break;
	}
}
