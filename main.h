#ifndef main_h__
#define main_h__

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
extern u8 spr_id, joy0, joy1;
#pragma zpsym("spr_id")

#define CLAMP(x, min, max) MAX(min, MIN(x, max))

#define SPR_FLIP_X 64
#define SPR_FLIP_Y 128
#define MSPRITE_END 128

typedef struct {
	s16 dx, dy;
	const u8 *msprite;
	
	// 8.8 fixed point.
	u16 x, y;
	s16 vx, vy;
} Ship;

extern Ship SHIP;

void ship_init(void);
void ship_update(void);

#endif main_h__