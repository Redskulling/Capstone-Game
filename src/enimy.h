#pragma once
#include <raylib.h>
#include "../math/Vector2.h"
#include "stats.h"

class Enimy {
public:
	v2f pos;
	f32 forwardVel;

	v2f leftAxis;

	v2f size;

	Stats stats;

public:
	inline Enimy(v2f pos, Stats stats, v2f size) : pos(pos), stats(stats), size(size) {};
	void Update();
};