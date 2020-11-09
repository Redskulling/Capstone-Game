#include "random.h"

#include <time.h>
u32 lehmerSeed = time(NULL);

u32 lehmer32() {
	lehmerSeed += 0xe120fc15;
    u64 tmp;
    tmp = (u64) lehmerSeed * 0x4a39b70d;
    u64 m1 = (tmp >> 32) ^ tmp;
    tmp = (u64) m1 * 0x12fad5c9;
    u32 m2 = (tmp >> 32) ^ tmp;
    return m2;
}

u32 randomUInt() {
	return lehmer32();
}

s32 randomInt(s32 min, s32 max) {
	return(lehmer32() % (max - min)) + min;
}

f32 randomFloat(f32 min, f32 max) {
    if (min >= max)
        return min;
    return( (( (f32) lehmer32() / (f32) MAX_INT)) * (max-min) + min);
}