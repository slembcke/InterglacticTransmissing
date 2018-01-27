#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"
#include "snake.h"

u8 i, ix, iy;
u8 joy0;

static const u8 MAIN_PALETTE[] = {
	0x0D, 0x20, 0x11, 0x16,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	
	0x0D, 0x20, 0x11, 0x16,
	0x0D, 0x20, 0x1A, 0x16,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
};

// Makes the compiler happy.
static TAIL_CALL TERMINATOR(void){return TERMINATOR();}

TAIL_CALL game_loop_start(void){
	ppu_off(); {
		pal_all(MAIN_PALETTE);
		
		vram_inc(0);
		vram_adr(NTADR_A(0, 0));
		vram_fill(0x00, 32*32);

		oam_clear();
	} ppu_on_all();
	
	music_play(0);
	
	ship_init();
	snake_init();
	
	while(true){
		static u8 mask;
		
		mask = PPU.mask;
		PPU.mask = mask | 0x01;
		
		spr_id = 0;
		
		joy0 = joy_read(0);
		
		if(JOY_START(joy0)){
			// move snake event in here to trigger from button press
		}
		snake_event(SHIP.x>>9, SHIP.y>>9, SHIP.vx, SHIP.vy);
		ship_update();
		snake_task();
		
		oam_hide_rest(spr_id);
		snake_draw_task();
		PPU.mask = mask;
		ppu_wait_nmi();
		snake_draw_post();
	}
	
	return TERMINATOR();
}

static const u8 TEXT_PALETTE[] = {
	0x0D, 0x20, 0x11, 0x16,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
};

static const char HEX[] = "0123456789ABCDEF";

void chr_debug(){
	ppu_off(); {
		pal_all(TEXT_PALETTE);
		oam_clear();
		
		vram_inc(0);
		vram_adr(NAMETABLE_A);
		vram_fill(0x00, 32*30);
		
		vram_adr(NTADR_A(8, 6));
		for(i = 0; i < 16; ++i) vram_put(HEX[i]);
		
		vram_inc(1);
		vram_adr(NTADR_A(6, 8));
		for(i = 0; i < 16; ++i) vram_put(HEX[i]);
		
		vram_adr(NTADR_A(7, 7));
		vram_put(0x14);
		vram_fill(0x0E, 16);
		vram_inc(0);
		vram_adr(NTADR_A(8, 7));
		vram_fill(0x0B, 16);
		
		for(iy = 0; iy < 16; ++iy){
			vram_adr(NTADR_A(8, iy + 8));
			for(ix = 0; ix < 16; ++ix){
				vram_put((iy << 4) | ix);
			}
		}
	} ppu_on_all();
	
	music_stop();
	
	while(true){
		// joy = joy_read(0);
		// if(JOY_START(joy)){
		// 	sfx_play(0, 0);
		// 	delay(60);
		// }
		
		ppu_wait_nmi();
	}
}

void main (void) {
	joy_install(joy_static_stddrv);
	
	bank_bg(0);
	bank_spr(1);
	
	// chr_debug();
	game_loop_start();
}
