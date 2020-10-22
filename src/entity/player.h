#pragma once
#include "../commonincludes.h"
#include "../../math/Vector2.h"
#include "playerstates.h"
#include "stats.h"
#include "../item/item.h"

#include "entity.h"

struct Circle {
	v2i centre;
	f32 radius;
};

class Player : public Entity {
public:
	f32 forwardVel;
	
	u32 input;
	v2f rightAxis;
	v2f triggerAxis;

	v2f prevLeftAxis;
	v2f prevRightAxis;

	s32 gamepad;

	f32 dashTimer;

	u32 state;
	Circle hitbox;
	Map *map;

	Inventory inventory;
	std::vector<Item *> &itemDrops;
public:
	Player(v2f pos, s32 gamepad, f32 radius, std::vector<Item *> &item);
	// Player();
	// ~Player();

public:
	void Draw();
	void Update(Map *map);
	virtual void Update(Player *p, std::vector<Entity *> e, Map *map);

public:
	void setState(u32 state);
	void runState(Map *map);

private:	
	void stateStationary();
	void stateMoving();
	void stateDash();
	void stateAttacking();

	bool checkCollision(Entity *e);
	bool collideItems();
};

void getPlayerInput(Player *player);

f32 fixGamepadAxisDeadzone(s32 gamepad, s32 axis, f32 deadzoneSize);