#include "player.h"

Player::Player(f32 x, f32 y, s32 gamepad, f32 radius) : gamepad(gamepad) {
	this->pos.x = x;
	this->pos.y = y;
	this->hitbox.radius = radius;
	this->hitbox.centre = { MAX_NEG_INT, MAX_NEG_INT };
}

Player::Player() {
	this->pos.x = 0;
	this->pos.y = 0;
	this->gamepad = GAMEPAD_PLAYER1;
	this->hitbox.radius = 30.0f;
	this->hitbox.centre = { MAX_NEG_INT, MAX_NEG_INT };
}

Player::~Player() {}

f32 fixGamepadAxisDeadzone(s32 gamepad, s32 axis, f32 deadzoneSize) {
	f32 movement = GetGamepadAxisMovement(gamepad, axis);
	if (movement > 0)
		if (movement <= deadzoneSize)
			return 0;
	if (movement < 0)
		if (movement >= -deadzoneSize)
			return 0;

	return movement;
}

void getPlayerInput(Player *p) {

	// Reset Input
	p->input = 0;
	p->leftAxis = { 0, 0 };

	// AXIS INPUT
	p->leftAxis.x = fixGamepadAxisDeadzone(p->gamepad, GAMEPAD_AXIS_LEFT_X, 0.15f);
	p->leftAxis.y = fixGamepadAxisDeadzone(p->gamepad, GAMEPAD_AXIS_LEFT_Y, 0.15f);

	f32 temp1 = fixGamepadAxisDeadzone(p->gamepad, GAMEPAD_AXIS_RIGHT_X, 0.15f);
	f32 temp2 = fixGamepadAxisDeadzone(p->gamepad, GAMEPAD_AXIS_RIGHT_Y, 0.15f);

	p->rightAxis = { temp1, temp2 };

	if (!temp1 && !temp2)
		p->rightAxis = p->leftAxis;

	if (!p->rightAxis) 	{
		v2f temp = GetMousePosition();
		p->rightAxis = { temp.x - GetScreenWidth()/2.0f, temp.y - GetScreenHeight()/2.0f };
	}

	p->rightAxis = p->rightAxis.norm();

	if (p->leftAxis.x == 0 && p->leftAxis.y == 0) {
		p->leftAxis.x = ((f32) (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (f32) (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)));
		p->leftAxis.y = ((f32) (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) - (f32) (IsKeyDown(KEY_UP)   || IsKeyDown(KEY_W)));
	}

	// BUTTON INPUT
	if (IsGamepadButtonDown(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsKeyDown(KEY_X) || IsKeyPressed(KEY_E))
		p->input |= INPUT_DASH_DOWN;
	
	if (IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsKeyPressed(KEY_X) || IsKeyPressed(KEY_E))
		p->input |= INPUT_DASH_PRESSED;

	if ((GetGamepadAxisMovement(p->gamepad, GAMEPAD_AXIS_LEFT_TRIGGER) >= 0 && p->triggerAxis.x < 0) || IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) ||
	   IsKeyPressed(KEY_Z) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		p->input |= INPUT_ATTACK;
	}

	p->triggerAxis.x = GetGamepadAxisMovement(p->gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);

}

void setPlayerState(Player *p, u32 state) {
	p->state = state;
}