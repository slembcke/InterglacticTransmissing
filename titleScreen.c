#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"
#include "snake.h"

static const u8 SHIP_RIGHT_MSPRITE[] = {
	-8, -8, 0x82, 0,
	 0, -8, 0x83, 0,
	-8,  0, 0x92, 0,
	 0,  0, 0x93, 0,
	MSPRITE_END
};

static const char TITLE_TEXT[] = "Interglactic Transmissing!";
static const char START_TEXT_1[] = "One Player";
static const char START_TEXT_2[] = "Two Player";
static const char START_TEXT_3[] = "Credits";
static const char GGJ_TEXT[] = "GGJ 2018";

#define STAR_COUNT 20
static u8 starY[STAR_COUNT] = {148, 132, 90, 9, 51, 56, 39, 151, 177, 9, 91, 193, 80, 86, 116, 84, 13, 42, 75, 46};
static const u8 starSprites[] = {0x7F, 0x8E, 0x8F, 0x9E, 0x9F};

u8 start_cursor = 0;

void draw_title_stars(void){
	static clock;
	clock = (nesclock() & 31) >> 2;
	
	for(i = 0; i < STAR_COUNT; ++i){
		spr_id = oam_spr(starY[i], i << 4, starSprites[ (clock + i) % 5], 3 | 32, spr_id);
		starY[i] -= i & 0x01 ? 2 : 4;
	}
}

TAIL_CALL title_loop_start(void){
	u8 x;
	u8 y;

	ppu_off(); {
		pal_all(MAIN_PALETTE);
		
		vram_inc(0);
		vram_adr(NTADR_A(0, 0));
		vram_fill(0x00, 32*32);

		oam_clear();

		// Various text on title screen
		vram_adr(NTADR_A(3, 8));
		vram_write(TITLE_TEXT, sizeof(TITLE_TEXT) - 1);
		
		pal_bg(MAIN_PALETTE + 8);
		// background tiles:
		for(x = 0; x < 2; ++x){
			for(y = 0; y < 2; ++y){
				const u8 dy = 15;
				DRAWTILE( 2 + 13*x,		2 + dy * y, 		0x80);
				DRAWTILE( 7 + 13*x,		3 + dy * y, 		0x82);
				DRAWTILE( 9 + 13*x,		1 + dy * y, 		0x84);
				DRAWTILE( 13 + 13*x,	10 + dy * y, 		0x86);
				DRAWTILE( 2 + 13*x,		6 + dy * y,			0x88);
				DRAWTILE( 3 + 13*x,		10 + dy * y, 		0xA0);
				DRAWTILE( 11 + 13*x,	4+ dy * y, 			0xA2);
				DRAWTILE( 8 + 13*x,		13+ dy * y, 		0x88);
			}
		} 
		
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
		vram_adr(NTADR_A(23, 28));
		vram_write(GGJ_TEXT, sizeof(GGJ_TEXT) - 1);


	} ppu_on_all();
	
	music_select(0);
	music_play(0);

	while(true){
		static u8 mask;
	 	static u8 clock;
		static u8 getDPAD = 1;
	 	
#ifdef DEBUG
		mask = PPU.mask;
		PPU.mask = mask | 0x01;
#endif
		
		spr_id = 0;
		
		joy0 = joy_read(0);
		
		if(JOY_START(joy0)){
			switch(start_cursor){
				case 0: TWO_PLAYER = false; return story_scroller_start();
				case 1: TWO_PLAYER = true; return story_scroller_start();
				default: return credits_start();
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
#ifdef DEBUG
		PPU.mask = mask;
#endif
		// Draw selection cursor:
		clock = (nesclock() >> 1) & 15;
		if(clock > 8) clock = 16 - clock;
		oam_meta_spr_pal( (7 * 8 - 4) + clock, (20 * 8 + 2) + 16 * start_cursor, 0, SHIP_RIGHT_MSPRITE);
		
		draw_title_stars();

		ppu_wait_nmi();

	}

	return game_loop_start();
}
