#pragma once
#include "../types.h"
#include <string>
#include <array>
#include "../../math/Vector2.h"
#include "../entity/entity.h"

struct Rectangle;

struct Item : public Entity {
	std::string name;
	s32 count;

	Item();
	Item(const std::string &name, s32 count, v2f pos, s32 id);
	~Item();

	void Draw();
	Rectangle Rect();
};

struct Slot {
	Item item;

	Slot(const Item &item);
	Slot();
	~Slot();
};

struct Inventory {
	std::array<Slot, 15> slots;
	std::array<Slot, 4>  armourSlots;
	std::array<Slot, 2>  holdSlots;

	u8 addItem(Item &item);
	u8 removeItem(Item &item);

	Inventory();
};


// struct ItemDrop : public Item {
// 	ItemDrop(const Item &item);
// };