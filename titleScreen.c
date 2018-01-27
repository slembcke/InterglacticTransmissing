#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"
#include "snake.h"

static const u8 MAIN_PALETTE[] = {
	0x0D, 0x20, 0x11, 0x16, // spaceship colors.
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x27, 0x07, 0x17, // asteroid colors
	0x0D, 0x21, 0x21, 0x21, // blue for the text.
	
	0x0D, 0x20, 0x11, 0x16,
	0x0D, 0x20, 0x1A, 0x16,
	0x0D, 0x00, 0x10, 0x20,
	0x0D, 0x00, 0x10, 0x20,
};


static const u8 SHIP_RIGHT_MSPRITE[] = {
	-8, -8, 0x82, 0,
	 0, -8, 0x83, 0,
	-8,  0, 0x92, 0,
	 0,  0, 0x93, 0,
	MSPRITE_END
};

static const char TITLE_TEXT[] = "Intergalactic Transmissing!";
static const char START_TEXT_1[] = "Easy";
static const char START_TEXT_2[] = "Hard";
static const char START_TEXT_3[] = "Credits";

u8 start_cursor = 0;

TAIL_CALL title_loop_start(void){
	u8 go = 1;

	ppu_off(); {
		pal_all(MAIN_PALETTE);
		
		vram_inc(0);
		vram_adr(NTADR_A(0, 0));
		vram_fill(0x00, 32*32);

		oam_clear();

		// Various text on title screen
		vram_adr(NTADR_A(4, 8));
		vram_write(TITLE_TEXT, sizeof(TITLE_TEXT) - 1);
		
		// set pallete to 0000 0101. top two of the tile are set to the 4th palette in the main palette above.
		vram_adr(NTADR_A(8 * 2, 30));
		vram_fill(0x05, 8); 

		// also change it for the text options:
		// vram_adr(NTADR_A(8 * 2, 30));
		// vram_fill(0x05, 8); 

		vram_adr(NTADR_A(9, 20));
		vram_write(START_TEXT_1, sizeof(START_TEXT_1) - 1);
		vram_adr(NTADR_A(9, 22));
		vram_write(START_TEXT_2, sizeof(START_TEXT_2) - 1);
		vram_adr(NTADR_A(9, 24));
		vram_write(START_TEXT_3, sizeof(START_TEXT_3) - 1);


		pal_bg(MAIN_PALETTE + 8);
		// background tiles:
		DRAWTILE( 2,	2, 0x80);
		DRAWTILE( 5,	3, 0x82);
		DRAWTILE( 10,	1, 0x84);
		DRAWTILE( 13,	10, 0x86);
		DRAWTILE( 2,	6, 0x88);
		DRAWTILE( 3,	10, 0xA0);
		DRAWTILE( 18,	4, 0xA2);
		DRAWTILE( 16,	10, 0x88);

	} ppu_on_all();
	
	music_play(0);

	while(go){
		static u8 mask;
	 	static u8 clock1;
		static u8 getDPAD = 1;
	 	
		mask = PPU.mask;
		PPU.mask = mask | 0x01;
		
		spr_id = 0;
		
		joy0 = joy_read(0);
		
		if(JOY_START(joy0)){
			if(start_cursor == 2){
				return chr_debug();
			}
			return game_loop_start();		
		} else if(JOY_DOWN(joy0)){
			if(getDPAD){
				sfx_play(0, 0);
				getDPAD = 0;
				start_cursor = (start_cursor + 1); // modulus into valid range.
				if(start_cursor > 2){
					start_cursor = 0;
				}
			}
		}else if(JOY_UP(joy0)){
			if(getDPAD){
				sfx_play(0, 0);
				getDPAD = 0;
				start_cursor = (start_cursor - 1);
				if(start_cursor > 2){
					start_cursor = 2;
				}
			}
		}else{
			getDPAD = 1;
		}
		
		oam_hide_rest(spr_id);
		PPU.mask = mask;

		clock1 = (nesclock() & 31) >> 2;
		// Draw selection cursor:
		oam_meta_spr_pal( (7 * 8 - 4) + clock1, (20 * 8 + 2) + 16 * start_cursor, 0, SHIP_RIGHT_MSPRITE);

		ppu_wait_nmi();

	}

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
