#pragma once
#include <stdint.h>

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef double f64;
typedef float f32;


#define MAX_INT 2147483647
#define MAX_NEG_INT -2147483648


#define INPUT_DASH_DOWN      0x0001
#define INPUT_DASH_PRESSED   0x0002
#define INPUT_ATTACK         0x0004
#define INPUT_SPRINT_PRESSED 0x0008
#define INPUT_ITEM_1         0x0010
#define INPUT_ITEM_2         0x0020
#define INPUT_PAUSE          0x0040

#define POTION_HEALTH  1
#define POTION_DEFENCE 2
#define POTION_ATTACK  3
#define POTION_STAMINA 4

extern s32 gTimeStop;
#define TIME_STOP_ENABLED 0x01
#define TIME_STOP_ALL     0x02
