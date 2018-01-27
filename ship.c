#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"

static const u8 TEXT_PALETTE[] = {
	0x0D, 0x20, 0x11, 0x16,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	
	0x0D, 0x20, 0x11, 0x16,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
};

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

static const u8 *SHIP_DIRECTIONS[] = {
	SHIP_UP_MSPRITE,
	SHIP_DOWN_MSPRITE,
	SHIP_LEFT_MSPRITE,
	SHIP_RIGHT_MSPRITE,
};

static struct {
	u8 direction;
	
	// 8.8 fixed point.
	u16 x, y;
	s16 vx, vy;
} Ship;

#define SHIP_ACCEL 16

TAIL_CALL ship_demo_start(){
	ppu_off(); {
		pal_all(TEXT_PALETTE);
		oam_clear();
		
		vram_inc(0);
		vram_adr(NAMETABLE_A);
		vram_fill(0x00, 32*32);
	} ppu_on_all();
	
	Ship.direction = DIRECTION_UP;
	Ship.x = Ship.y = (128 << 8);
	Ship.vx = Ship.vy = 0;
	
	while(true){
		register s16 dx, dy;
		
		joy = joy_read(0);
		dx = (JOY_LEFT(joy) ? -256 : 0) + (JOY_RIGHT(joy) ? 256 : 0);
		dy = (JOY_UP(joy) ? -256 : 0) + (JOY_DOWN(joy) ? 256 : 0);
		
		Ship.vx = Ship.vx + CLAMP(dx - Ship.vx, -SHIP_ACCEL, SHIP_ACCEL);
		Ship.vy = Ship.vy + CLAMP(dy - Ship.vy, -SHIP_ACCEL, SHIP_ACCEL);
		
		Ship.x += Ship.vx;
		Ship.y += Ship.vy;
		
		spr_id = 0;
		spr_id = oam_meta_spr((Ship.x >> 8), (Ship.y >> 8), spr_id, SHIP_DIRECTIONS[Ship.direction]);
		oam_hide_rest(spr_id);
		
		ppu_wait_nmi();
	}
	
	return ship_demo_start();
}
