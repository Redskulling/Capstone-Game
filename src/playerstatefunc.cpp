#include "playerstatefunc.h"


void playerStateStationary(Player *p) {
	if (p->leftAxis)
		return setPlayerState(p, PLAYER_STATE_MOVING);

	if (p->input & INPUT_ATTACK) {
		p->dashTimer = 0.125f;
		return setPlayerState(p, PLAYER_STATE_ATTACKING);
	}
}

void playerStateMoving(Player *p) {
	p->pos += p->leftAxis * (p->forwardVel * p->deltaTime);

	if (p->input & INPUT_DASH_PRESSED) {
		p->dashTimer = 0.125f;
		return setPlayerState(p, PLAYER_STATE_DASH);
	}
	
	if (p->input & INPUT_ATTACK) {
		p->dashTimer = 0.125f;
		return setPlayerState(p, PLAYER_STATE_ATTACKING);
	}

	if (!p->leftAxis)
		return setPlayerState(p, PLAYER_STATE_STATIONARY);

}

void playerStateDash(Player *p) {
	v2f ceilLeftAxis = {(p->leftAxis.x > 0 ? ceilf32(p->leftAxis.x) : floorf32(p->leftAxis.x)), 
		(p->leftAxis.y > 0 ? ceilf32(p->leftAxis.y) : floorf32(p->leftAxis.y)) };

	p->pos += ceilLeftAxis * 500.0f * p->deltaTime;
	if ((p->dashTimer -= p->deltaTime) <= 0) {
		p->dashTimer = 0;
		return setPlayerState(p, PLAYER_STATE_MOVING);
	}

	if (p->input & INPUT_ATTACK) {
		p->dashTimer = 0.125f;
		return setPlayerState(p, PLAYER_STATE_ATTACKING);
	}
}

void playerStateAttacking(Player *p) {
	p->hitbox.centre = (v2i) ((p->pos + v2f{ 25.0f, 25.0f }) + (p->rightAxis * p->hitbox.radius * 2.0f));
	if ((p->dashTimer -= p->deltaTime) <= 0) {
		p->dashTimer = 0;
		p->hitbox.centre = { MAX_NEG_INT, MAX_NEG_INT };
		setPlayerState(p, PLAYER_STATE_MOVING);
	}
}

void runPlayerState(Player *p) {
	switch (p->state) {
		case PLAYER_STATE_MOVING:     playerStateMoving(p);     break;
		case PLAYER_STATE_DASH:       playerStateDash(p);       break;
		case PLAYER_STATE_STATIONARY: playerStateStationary(p); break;
		case PLAYER_STATE_ATTACKING:  playerStateAttacking(p);  break;
	}
}
