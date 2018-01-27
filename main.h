#include <stdbool.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

// Dummy return type to enforce tail calling for gamestates.
typedef struct {} TAIL_CALL;

extern u8 i, ix, iy;
extern u8 spr_id, joy;

#define CLAMP(x, min, max) MAX(min, MIN(x, max))

#define SPR_FLIP_X 64
#define SPR_FLIP_Y 128
#define MSPRITE_END 128

enum {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_RIGHT,
	DIRECTION_LEFT,
} DIRECTION;
