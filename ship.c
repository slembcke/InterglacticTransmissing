#include <string.h>
#include <stdlib.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"

static const u8 SHIP_UP_MSPRITE[] = {
	-8, -8, 0x80, 0,
	 0, -8, 0x81, 0,
	-8,  0, 0x90, 0,
	 0,  0, 0x91, 0,
	MSPRITE_END
};

static const u8 SHIP_DOWN_MSPRITE[] = {
	-8, -8, 0x90, SPR_FLIP_Y,
	 0, -8, 0x91, SPR_FLIP_Y,
	-8,  0, 0x80, SPR_FLIP_Y,
	 0,  0, 0x81, SPR_FLIP_Y,
	MSPRITE_END
};

static const u8 SHIP_RIGHT_MSPRITE[] = {
	-8, -8, 0x82, 0,
	 0, -8, 0x83, 0,
	-8,  0, 0x92, 0,
	 0,  0, 0x93, 0,
	MSPRITE_END
};

static const u8 SHIP_LEFT_MSPRITE[] = {
	-8, -8, 0x83, SPR_FLIP_X,
	 0, -8, 0x82, SPR_FLIP_X,
	-8,  0, 0x93, SPR_FLIP_X,
	 0,  0, 0x92, SPR_FLIP_X,
	MSPRITE_END
};

static const u8 SHIP_RIGHT_UP_MSPRITE[] = {
	-8, -8, 0x84, 0,
	 0, -8, 0x85, 0,
	-8,  0, 0x94, 0,
	 0,  0, 0x95, 0,
	MSPRITE_END
};

static const u8 SHIP_RIGHT_DOWN_MSPRITE[] = {
	-8, -8, 0x94, SPR_FLIP_Y,
	 0, -8, 0x95, SPR_FLIP_Y,
	-8,  0, 0x84, SPR_FLIP_Y,
	 0,  0, 0x85, SPR_FLIP_Y,
	MSPRITE_END
};

static const u8 SHIP_LEFT_UP_MSPRITE[] = {
	-8, -8, 0x85, SPR_FLIP_X,
	 0, -8, 0x84, SPR_FLIP_X,
	-8,  0, 0x95, SPR_FLIP_X,
	 0,  0, 0x94, SPR_FLIP_X,
	MSPRITE_END
};

static const u8 SHIP_LEFT_DOWN_MSPRITE[] = {
	-8, -8, 0x95, SPR_FLIP_X | SPR_FLIP_Y,
	 0, -8, 0x94, SPR_FLIP_X | SPR_FLIP_Y,
	-8,  0, 0x85, SPR_FLIP_X | SPR_FLIP_Y,
	 0,  0, 0x84, SPR_FLIP_X | SPR_FLIP_Y,
	MSPRITE_END
};

// Map ship directions directly to the right, left, down, up input bits.
// NULL if conflict. (ex: right and left)
static const u8 * const SHIP_DIRECTIONS[] = {
	NULL,
	SHIP_UP_MSPRITE,
	SHIP_DOWN_MSPRITE,
	NULL,
	SHIP_LEFT_MSPRITE,
	SHIP_LEFT_UP_MSPRITE,
	SHIP_LEFT_DOWN_MSPRITE,
	SHIP_LEFT_MSPRITE,
	SHIP_RIGHT_MSPRITE,
	SHIP_RIGHT_UP_MSPRITE,
	SHIP_RIGHT_DOWN_MSPRITE,
	SHIP_RIGHT_MSPRITE,
	NULL,
	SHIP_UP_MSPRITE,
	SHIP_DOWN_MSPRITE,
	NULL,
};

static struct {
	s16 dx, dy;
	const u8 *msprite;
	
	// 8.8 fixed point.
	u16 x, y;
	s16 vx, vy;
} Ship;

#define SHIP_SPEED 512
#define SHIP_ACCEL 32

#define BOUNDS_L ( 16 << 8)
#define BOUNDS_R (240 << 8)
#define BOUNDS_T ( 16 << 8)
#define BOUNDS_B (224 << 8)

void ship_init(void){
	Ship.msprite = SHIP_UP_MSPRITE;
	Ship.x = Ship.y = (128 << 8);
	Ship.vx = Ship.vy = 0;
}

void ship_update(void){
	static const u8 *msprite;
	
	msprite = SHIP_DIRECTIONS[joy0 >> 4];
	if(msprite) Ship.msprite = msprite;
	
	Ship.dx = (JOY_LEFT(joy0) ? -SHIP_SPEED : 0) + (JOY_RIGHT(joy0) ? SHIP_SPEED : 0);
	Ship.dy = (JOY_UP(joy0) ? -SHIP_SPEED : 0) + (JOY_DOWN(joy0) ? SHIP_SPEED : 0);
	
	Ship.vx = Ship.vx + CLAMP(Ship.dx - Ship.vx, -SHIP_ACCEL, SHIP_ACCEL);
	Ship.vy = Ship.vy + CLAMP(Ship.dy - Ship.vy, -SHIP_ACCEL, SHIP_ACCEL);
	
	Ship.x += Ship.vx;
	Ship.y += Ship.vy;
	
	if(Ship.x < BOUNDS_L){
		Ship.vx = abs(Ship.vx >> 1);
		Ship.x = BOUNDS_L;
	}
	
	if(Ship.x > BOUNDS_R){
		Ship.vx = -abs(Ship.vx >> 1);
		Ship.x = BOUNDS_R;
	}
	
	if(Ship.y < BOUNDS_T){
		Ship.vy = abs(Ship.vy >> 1);
		Ship.y = BOUNDS_T;
	}
	
	if(Ship.y > BOUNDS_B){
		Ship.vy = -abs(Ship.vy >> 1);
		Ship.y = BOUNDS_B;
	}
	
	oam_meta_spr_pal((Ship.x >> 8), (Ship.y >> 8), 0, Ship.msprite);
}
