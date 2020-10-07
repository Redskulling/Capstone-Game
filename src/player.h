#pragma once
#include "types.h"
#include <raylib.h>
#include "../math/Vector2.h"
#include "playerstates.h"
#include "stats.h"

#include "entity.h"

struct Circle {
	v2i centre;
	f32 radius;
};

class Player : public Entity {
public:
	f32 forwardVel;
	
	u32 input;
	v2f leftAxis;
	v2f rightAxis;
	v2f triggerAxis;

	v2f prevLeftAxis;
	v2f prevRightAxis;

	s32 gamepad;

	f32 dashTimer;

	u32 state;
	Circle hitbox;

public:
	Player(v2f pos, s32 gamepad, f32 radius);
	Player();
	// ~Player();

public:
	void Draw();
	void Update();
	virtual void Update(Player *p, std::vector<Entity *> e);
	bool checkCollision(Entity *e);

private:
};

void getPlayerInput(Player *player);

f32 fixGamepadAxisDeadzone(s32 gamepad, s32 axis, f32 deadzoneSize);

void setPlayerState(Player *player, u32 state);