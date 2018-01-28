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

// Fire offsets.
static const u8 FX[] = {
	0, -4, -4, 0,
	6, 1, 1, 6,
	-14, -9, -9, -14,
	0, -4, -4, 0,
};

static const u8 FY[] = {
	0, 6, -14, 0,
	-4, 1, -9, -4,
	-4, 1, -9, -4,
	0, 6, -14, 0,
};

Ship SHIP[2];

#define SHIP_SPEED 512
#define SHIP_ACCEL 32

#define BOUNDS_L ( 8 << 8)
#define BOUNDS_R (248 << 8)
#define BOUNDS_T ( 8 << 8)
#define BOUNDS_B (232 << 8)

void ship_init(void){
	SHIP[0].msprite = SHIP_UP_MSPRITE;
	SHIP[0].x = SHIP[0].y = (112 << 8);
	SHIP[0].vx = SHIP[0].vy = 0;
	
	SHIP[1].msprite = SHIP_UP_MSPRITE;
	SHIP[1].x = SHIP[1].y = (144 << 8);
	SHIP[1].vx = SHIP[1].vy = 0;
}

void ship_update(u8 joy, u8 ship_idx){
	static u8 fire;
	static const u8 *msprite;
	static Ship ship;
	ship = SHIP[ship_idx];
	
	msprite = SHIP_DIRECTIONS[joy >> 4];
	if(msprite) ship.msprite = msprite;
	
	ship.dx = (JOY_LEFT(joy) ? -SHIP_SPEED : 0) + (JOY_RIGHT(joy) ? SHIP_SPEED : 0);
	ship.dy = (JOY_UP(joy) ? -SHIP_SPEED : 0) + (JOY_DOWN(joy) ? SHIP_SPEED : 0);
	
	ship.vx = ship.vx + CLAMP(ship.dx - ship.vx, -SHIP_ACCEL, SHIP_ACCEL);
	ship.vy = ship.vy + CLAMP(ship.dy - ship.vy, -SHIP_ACCEL, SHIP_ACCEL);
	
	ship.x += ship.vx;
	ship.y += ship.vy;
	
	if(ship.x < BOUNDS_L){
		ship.vx = abs(ship.vx >> 1);
		ship.x = BOUNDS_L;
	}
	
	if(ship.x > BOUNDS_R){
		ship.vx = -abs(ship.vx >> 1);
		ship.x = BOUNDS_R;
	}
	
	if(ship.y < BOUNDS_T){
		ship.vy = abs(ship.vy >> 1);
		ship.y = BOUNDS_T;
	}
	
	if(ship.y > BOUNDS_B){
		ship.vy = -abs(ship.vy >> 1);
		ship.y = BOUNDS_B;
	}
	
	SHIP[ship_idx] = ship;
	
	oam_meta_spr_pal((ship.x >> 8), (ship.y >> 8), ship_idx, ship.msprite);
	
	if(joy & 0xF0){
		register fx = (ship.x >> 8) + FX[joy >> 4], fy = (ship.y >> 8) + FY[joy >> 4];
		spr_id = oam_spr(fx, fy, 0xCC + (fire & 0x3), ((fire & 0xC) << 4) | (!ship_idx), spr_id);
		++fire;
	}
}
