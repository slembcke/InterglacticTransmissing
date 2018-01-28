#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"
#include "snake.h"

bool TWO_PLAYER;
u8 i, ix, iy;
u8 joy0, joy1;

const u8 MAIN_PALETTE[] = {
	0x0D, 0x20, 0x11, 0x16, // spaceship colors. Probably not really used.
	0x0D, 0x00, 0x10, 0x20, // greyscale stuff.. ? not sure what this is for.
	0x0D, 0x27, 0x07, 0x17, // asteroid colors
	0x0D, 0x21, 0x21, 0x21, // blue for the text.
	
	0x0D, 0x20, 0x11, 0x01, // P1 palette.
	0x0D, 0x20, 0x17, 0x07, // P2 palette.
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
};

const u8 asteroidCornerIndex[] = {0x80,0x82,0x84,0x86,0x88,0xA0,0xA2,0x80};

TAIL_CALL game_loop_start(void){
	ppu_off(); {
		pal_all(MAIN_PALETTE);
		pal_bright(4);

		vram_inc(0);
		vram_adr(NTADR_A(0, 0));
		vram_fill(0x00, 32*32);

		snake_init();

		oam_clear();
	} ppu_on_all();
	
	music_select(1);
	music_play(0);
	
	ship_init();
	
	while(true){
		static u8 mask;
		
#ifdef DEBUG
		mask = PPU.mask;
		// PPU.mask = mask | 0x01;
#endif		
		spr_id = 0;
		
		joy0 = joy_read(0);
		joy1 = joy_read(1);
		
		if(JOY_SELECT(joy0)){
			// restart current level:
			return end_level_sequence();
		}

		snake_event(SHIP[0].x>>9, SHIP[0].y>>9, SHIP[0].vx, SHIP[0].vy);
		ship_update(joy0, 0);
		
		if(TWO_PLAYER){
			snake_event(SHIP[1].x>>9, SHIP[1].y>>9, SHIP[1].vx, SHIP[1].vy);
			ship_update(joy1, 1);
		}
		
		snake_task();
		
		oam_hide_rest(spr_id);
		snake_draw_task();
#ifdef DEBUG
		PPU.mask = mask;
#endif
		ppu_wait_nmi();
		snake_draw_post();
	}
	
	return TERMINATOR();
}

#define FADE_SPEED 10
TAIL_CALL end_level_sequence(){
	music_stop();
	sfx_play(1, 0);
	delay(10);

	// Run some neat pallete changes.
	pal_bright(3);
	delay(FADE_SPEED);
	pal_bright(2);
	delay(FADE_SPEED);
	pal_bright(1);
	delay(FADE_SPEED);
	pal_bright(0);

	return game_loop_start();
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

TAIL_CALL chr_debug(){
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
		joy0 = joy_read(0);
		if(JOY_BTN_1(joy0)){
			bank_bg(0);
		}
		if(JOY_BTN_2(joy0)){
			bank_bg(1);
		}
		
		ppu_wait_nmi();
	}
	return TERMINATOR();
}

#define YOU_LOSE 0x8F
#define YOU_WIN 0x8E

void main_event(u8 event, void * data)
{
	if(event==YOU_WIN) {
		// NO need to handle this
	}
	else if(event==YOU_LOSE) {
		// I guess you lose?
		// queue lose event or handle
		end_level_sequence(); // should be a tail call
	}
}

void main (void) {
	joy_install(joy_static_stddrv);
	
	bank_bg(0);
	bank_spr(1);
	
	// bank_bg(1);
	// chr_debug();
	
	title_loop_start();
	game_loop_start();
}
