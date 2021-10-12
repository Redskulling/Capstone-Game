#include "player.h"
#include "../map/map.h"
#include "button.h"

Player::Player(v2f pos, s32 gamepad, f32 radius, std::vector<Item *> &item, Texture2D *texture)
		: itemDrops(item), invincibleTimer(0), Entity(pos, {10.0f, 10.0f, 5.0f, 5.0f, 1.0f, 0.0f, 1.0f}, {22, 28}, 0) {
	this->hitbox.radius = radius;
	this->hitbox.centre = { MAX_NEG_INT, MAX_NEG_INT };
	this->state = PLAYER_STATE_STATIONARY;
	this->texture = texture;
	this->stamina = 100.0f;
	this->maxStamina = 100.0f;
	this->xp = 0;
	this->nextLevel = 50;
	this->level = 1;
	this->attack = LoadTexture("img/attack.png");
}

Player::~Player() {
	delete map;
	UnloadTexture(*this->texture);
	UnloadTexture(this->attack);
}

void Player::Draw() {
	if (this->dead) return;

	if (this->rightAxis.x < 0)
		this->texture->width < 0 ? this->texture->width : this->texture->width *= -1;
	else if (this->rightAxis.x > 0)
		this->texture->width > 0 ? this->texture->width : this->texture->width *= -1;

	DrawTextureRec(*this->texture, { this->animRect.x * 32.0f, this->animRect.y * 32.0f, 32, 32 }, {this->pos.x - 4, this->pos.y - 3}, WHITE);
	DrawTextureV(this->attack, this->hitbox.centre - (v2i){16, 16}, WHITE);
}

void Player::Update(Map *map, std::vector<Entity *> &e) {
	this->runState(map, e);
}

void Player::Update(Player *p, std::vector<Entity *> &e, Map *map) {
	this->runState(map, e);
}

bool Player::checkCollision(Entity *e) {
	return 0;
}

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

	if (IsKeyDown(KEY_LEFT_SHIFT) || IsGamepadButtonDown(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
		p->input |= INPUT_SPRINT_PRESSED;
	}

	// BUTTON INPUT
	if (IsGamepadButtonDown(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP) || IsKeyDown(KEY_X) || IsKeyPressed(KEY_SPACE))
		p->input |= INPUT_DASH_DOWN;
	
	if (IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP) || IsKeyPressed(KEY_X) || IsKeyPressed(KEY_SPACE))
		p->input |= INPUT_DASH_PRESSED;

	if ((GetGamepadAxisMovement(p->gamepad, GAMEPAD_AXIS_LEFT_TRIGGER) >= 0 && p->triggerAxis.x < 0) ||
	    (GetGamepadAxisMovement(p->gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER) >= 0 && p->triggerAxis.y < 0)
	   || IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) ||
	   IsKeyPressed(KEY_Z) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		p->input |= INPUT_ATTACK;
	}

	if (IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1) ||
	    IsKeyPressed(KEY_Q))
		p->input |= INPUT_ITEM_1;

	if (IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ||
	    IsKeyPressed(KEY_E))
		p->input |= INPUT_ITEM_2;

	if (IsGamepadButtonPressed(p->gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
		p->input |= INPUT_PAUSE;

	p->triggerAxis.x = GetGamepadAxisMovement(p->gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);
	p->triggerAxis.y = GetGamepadAxisMovement(p->gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);
}

void Player::setState(u32 state) {
	this->state = state;
}

bool Player::collideItems() {
	for (Item *item : this->itemDrops) {
		if (item->name == "NULL")
			continue;
		if (CheckCollisionRecs(this->Rect(), item->Rect()))
			return this->inventory.addItem(*item);
	}
	return 0;
}

void Player::collide(std::vector<Entity *> &ent) {
	for (auto &e : ent) {
		if (e->id == this->id) continue;
		if (e->dead) {
			this->xp += e->xp;
			e->xp = 0;
			continue;
		}

		if (this->map->getTilePos(this->pos).type & 0x4) {
			this->map->NewMap(ent);
			return;
		}

		if (this->map->getTilePos(this->pos + this->size).type & 0x4) {
			this->map->NewMap(ent);
			return;
		}
		
		if (this->map->getTilePos(this->pos + this->size * v2f{1, 0}).type & 0x4) {
			this->map->NewMap(ent);
			return;
		}
		
		if (this->map->getTilePos(this->pos + this->size * v2f{0, 1}).type & 0x4) {
			this->map->NewMap(ent);
			return;
		}

		if (IsKeyPressed(KEY_F5)) {
			this->map->NewMap(ent);
			return;
		}

		Rectangle r = this->Rect();
		if ((CheckCollisionRecs({r.x - 1.0f, r.y - 1.0f, r.width + 1.5f, r.height + 1.5f}, e->Rect()) && this->invincibleTimer <= 0)) {
			this->invincibleTimer = 1;
			f32 dmg = calcDamage(e, this);
			dmg = dmg >= 0 ? dmg : 0;
			this->stats.hp -= dmg;
			if (this->stats.hp <= 0) {
				this->stats.maxhp = 10;
				this->maxStamina = 100;
				this->stamina = 100;
				this->stats.hp = this->stats.maxhp;
				this->stats.str = 1;
				this->stats.def = 1;
				this->map->NewMap(ent);
			}
			return;
		}
	}
}


void Player::drawPauseScreen() {
	f32 top, bottom, left, right, width, height;
	f32 screenHeight = (f32) GetScreenHeight();
	f32 screenWidth  = (f32) GetScreenWidth();
	f32 screenPop = 680.0f;
	Button buttons[2];
	if (screenHeight <= screenPop) {
		top = 0;
		height = screenHeight;
	} else {
		top    = (screenHeight * 0.5f) - (screenPop * 0.5f);
		height = screenPop;
	}
	if (screenWidth <= screenPop) {
		left = 0;
		width = screenWidth;
	} else {
		left   = (screenWidth * 0.5f) - (screenPop *0.5f);
		width  = screenPop;
	}
	bottom = top + height;
	right = left + width;

	DrawRectangle(left, top, width, height, GRAY);
	DrawText("PAUSE", left + 10, top + 10, 40, BLACK);
	buttons[0] = {left + 10, top + 55, width - 20, 20};
	buttons[0].draw(DARKGRAY);
	s32 buttonClick = buttons[0].detectClick((v2i) GetMousePosition());
	if (buttonClick) {
		DrawText("CLICK!", left + 10, top + 55, 40, BLACK);
	}


}