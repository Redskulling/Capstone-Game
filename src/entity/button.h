#pragma once
#include "../types.h"
#include "../../math/Vector2.h"

class Button {
public:
	f32 x, y, w, h;

	inline Button(f32 x, f32 y, f32 w, f32 h) : x(x), y(y), w(w), h(h) {};
	inline Button() : x(0), y(0), w(0), h(0) {};
	inline s32 detectClick(const v2i &mouse) {
		if (mouse.x >= x && mouse.x <= (w + x) && mouse.y >= y && mouse.y <= (h + y))
			return 1;
		return 0;
	}
	inline void draw(Color color) {
		DrawRectangle(x, y, w, h, color);
	}
};