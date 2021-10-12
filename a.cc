#include "math/Vector2.h"
#include "src/types.h"

#include <stdio.h>

int main(void) {
	v2f looka = { 0.67f, 0.74f };
	v2f posa  = { 500.0f, 6276.0f };
	v2f lookb = { -0.79f, -0.612f };
	v2f posb  = { 19880, 6181 };

	v2f save1 = { 0, 0 };
	v2f save2 = { 0, 0 };


	f32 a = looka.x / lookb.y;
	save1.x = a;
	a *= posa.x;
	a -= posa.y;
	save1.y = a;

	f32 b = lookb.x / lookb.y;
	save2.x = b;
	b *= posb.x;
	b -= posb.y;
	save2.y = b;
	
	v2f out = { 0, 0 };
	out.x = ((save2.x - save1.x) / (save1.y - save2.y));
	out.y = save1.y + (save1.x * out.x);

	printf("%f, %f\n", out.x, out.y);
}