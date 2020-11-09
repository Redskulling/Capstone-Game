#include "item.h"
#include "../entity/entity.h"
#include <raylib.h>


Item::Item() : name("NULL"), count(0), Entity({ 0, 0 }, {0, 0, 0, 0}, {32, 32}, -1) {}

Item::Item(const std::string &name, s32 count, v2f pos, s32 id, Texture2D *texture) : name(name), count(count), Entity(pos, {0, 0, 0, 0}, {32, 32}, id) {
	this->texture = texture;
}

Item::~Item() {}

void Item::Draw() {
	if (this->name == "HEAL")
		DrawTextureRec(*this->texture, {0, 0, 32, 32}, this->pos, WHITE);
	if (this->name == "ATT")
		DrawTextureRec(*this->texture, {32, 0, 32, 32}, this->pos, WHITE);
	if (this->name == "DEF")
		DrawTextureRec(*this->texture, {64, 0, 32, 32}, this->pos, WHITE);
	if (this->name == "STAM")
		DrawTextureRec(*this->texture, {96, 0, 32, 32}, this->pos, WHITE);
}

Rectangle Item::Rect() {
	return { this->pos.x, this->pos.y, this->size.x, this->size.y };
}

Slot::Slot(const Item &item) : item(item) {}

Slot::Slot() : item(Item()) {} 

Slot::~Slot() {}

Inventory::Inventory() : slots{{Slot()}}, armourSlots{{Slot()}}, holdSlots{{Slot()}} {}


u8 Inventory::addItem(Item &item) {
	for (int i = 0; i < this->slots.size(); i++) {
		if (item.name == "NULL" || item.count <= 0) {
			item.name = "NULL";
			item.count = 0;
			return 2;
		}
		if (this->slots[i].item.name == "NULL") {
			this->slots[i].item = item;
			item.name = "NULL";
			item.count = 0;
			return 1;
		} else if (this->slots[i].item.name == item.name) {
			this->slots[i].item.count += item.count;
			if (this->slots[i].item.count >= 100) {
				item.count = this->slots[i].item.count - 99;
				this->slots[i].item.count = 99;
				if (item.count <= 0)
					item.name = "NULL";
				continue;
			}
			item.count = 0;
		}
	}
	if (item.count == 0)
		item.name = "NULL";
	return 0;
}

u8 Inventory::removeItem(Item &item) {
	for (int i = 0; i < this->slots.size(); i++) {
		if (this->slots[i].item.name == item.name) {
			this->slots[i].item.count -= item.count;
			if (this->slots[i].item.count < 0) {
				item.count += this->slots[i].item.count;
				this->slots[i].item.count = 0;
				this->slots[i].item.name = "NULL";
				continue;
			}
			return 1;
		}
	}
	return 0;
}

u8 Inventory::useItem(u32 slot) {
	std::string &a = this->slots[slot].item.name;
	if (a == "HEAL") {
		this->slots[slot].item.count -= 1;
		if (this->slots[slot].item.count == 0) 
			this->slots[slot].item.name = "NULL";
		return 1;
	}
	else if (a == "ATT") {
		this->slots[slot].item.count -= 1;
		if (this->slots[slot].item.count == 0) 
			this->slots[slot].item.name = "NULL";
		return 2;
	}
	else if (a == "DEF") {
		this->slots[slot].item.count -= 1;
		if (this->slots[slot].item.count == 0) 
			this->slots[slot].item.name = "NULL";
		return 3;
	}
	else if (a == "STAM") {
		this->slots[slot].item.count -= 1;
		if (this->slots[slot].item.count == 0) 
			this->slots[slot].item.name = "NULL";
		return 4;
	}
	return 0;
}
// ItemDrop::ItemDrop(const Item &item, v2f pos) : Item(item), pos(pos) {

// }