#pragma once
#include "types.h"
#include <raylib.h>
#include "../math/Vector2.h"
#include "playerstates.h"
#include "stats.h"

struct Circle {
	v2i centre;
	f32 radius;
};

class Player {
public:
	v2f pos;
	f32 forwardVel;
	f32 deltaTime;
	
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

	Stats stats;

public:
	Player(f32 x, f32 y, s32 gamepad, f32 radius);
	Player();
	~Player();
};

void getPlayerInput(Player *player);

f32 fixGamepadAxisDeadzone(s32 gamepad, s32 axis, f32 deadzoneSize);

void setPlayerState(Player *player, u32 state);