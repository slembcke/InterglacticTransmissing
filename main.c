#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")

u8 oam_off;

#pragma data-name(pop)
#pragma bss-name (pop)

u8 i, ix, iy;
u8 spr_id, joy;

static const u8 DEMO_PALETTE[] = {
	0x0D, 0x0F, 0x0F, 0x30,
	0x0D, 0x0F, 0x0F, 0x05,
	0x0D, 0x0F, 0x0F, 0x09,
	0x0D, 0x0F, 0x0F, 0x01,
	0x0D, 0x00, 0x00, 0x36,
	0x0D, 0x00, 0x00, 0x37,
	0x0D, 0x00, 0x00, 0x38,
	0x0D, 0x00, 0x00, 0x39,
};

static const char GREET[] = "Global Game Jam 2018!!";
static const u8 CENTER_PAT[] = {0x00, 0x98, 0x09, 0x09, 0x01, 0x01, 0x8A, 0x8A, 0x18, 0x00};

static const s8 SIN_TABLE[256] = {
	0, 0, 1, 2, 3, 3, 4, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19, 19, 20, 20,
	21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 30, 31, 31, 31,
	31, 31, 31, 31, 31, 31, 31, 32, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 29, 29, 29, 28, 28,
	28, 27, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21, 20, 20, 19, 19, 18, 17, 17, 16, 15, 15, 14, 13,
	12, 12, 11, 10, 10, 9, 8, 7, 7, 6, 5, 4, 3, 3, 2, 1, 0, 0, 0, -1, -2, -3, -3, -4, -5, -6, -7, -7, -8, -9,
	-10, -10, -11, -12, -12, -13, -14, -15, -15, -16, -17, -17, -18, -19, -19, -20, -20, -21, -22, -22, -23,
	-23, -24, -24, -25, -25, -26, -26, -27, -27, -27, -28, -28, -28, -29, -29, -29, -30, -30, -30, -30, -31,
	-31, -31, -31, -31, -31, -31, -31, -31, -31, -32, -31, -31, -31, -31, -31, -31, -31, -31, -31, -31, -30,
	-30, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27, -27, -26, -26, -25, -25, -24, -24, -23, -23, -22,
	-22, -21, -20, -20, -19, -19, -18, -17, -17, -16, -15, -15, -14, -13, -12, -12, -11, -10, -10, -9, -8, -7,
	-7, -6, -5, -4, -3, -3, -2, -1, 0,
};

TAIL_CALL main_demo_start(void);
TAIL_CALL text_demo_start(void);
TAIL_CALL ship_demo_start();

TAIL_CALL main_demo_loop(void);
TAIL_CALL main_demo_start(void){
	ppu_off(); {
		pal_all(DEMO_PALETTE);
		oam_clear();
		
		vram_inc(0);
		
		// Fill screen with opaque blocks.
		vram_adr(NTADR_A(0, 0));
		vram_fill(128, 32*30);
		
		// Punch a hole for the GGJ text.
		vram_adr(NTADR_A(4, 3));
		vram_fill(0, sizeof(GREET) + 1);
		vram_adr(NTADR_A(4, 4));
		vram_fill(0, sizeof(GREET) + 1);
		vram_adr(NTADR_A(4, 5));
		vram_fill(0, sizeof(GREET) + 1);
		vram_adr(NTADR_A(5, 4));
		vram_write(GREET, sizeof(GREET) - 1);
		
		// Punch the hole in the middle.
		for(i = 0; i < 10; ++i){
			vram_adr(NTADR_A(11, 10 + i)); vram_write(CENTER_PAT, 10);
		}
		
		// Set the DEMO_PALETTE pattern for the color bars.
		vram_adr(NTADR_A(0, 30));
		vram_fill(0x50, 8);
		vram_fill(0xFA, 8);
		vram_fill(0x50, 8);
		vram_fill(0xFA, 8);
		vram_fill(0x50, 8);
		vram_fill(0xFA, 8);
		vram_fill(0x50, 8);
		vram_fill(0xFA, 8);
	} ppu_on_all();
	
	music_play(0);
	
	return main_demo_loop();
}

TAIL_CALL main_demo_loop(void){
	static u8 t0, t2, t3;
	static u8 c0 = 0, c1 = 3, c2 = 6, c3 = 9;
	static u8 mask;
	
	mask = PPU.mask;
	// PPU.mask = mask | 0x01;
	
	t0 = nesclock();
	t2 = 2*t0;
	t3 = 3*t0;
	
	spr_id = 0;
	spr_id = oam_spr(124 + SIN_TABLE[(t3 + 0x00) & 0xFF], 116 + SIN_TABLE[(t2 + 0x40) & 0xFF], '!', ((t3 + 0x00 + 0xC0) >> 2) & 0x20, spr_id);
	spr_id = oam_spr(124 + SIN_TABLE[(t3 + 0x10) & 0xFF], 116 + SIN_TABLE[(t2 + 0x50) & 0xFF], '8', ((t3 + 0x10 + 0xC0) >> 2) & 0x20, spr_id);
	spr_id = oam_spr(124 + SIN_TABLE[(t3 + 0x20) & 0xFF], 116 + SIN_TABLE[(t2 + 0x60) & 0xFF], '1', ((t3 + 0x20 + 0xC0) >> 2) & 0x20, spr_id);
	spr_id = oam_spr(124 + SIN_TABLE[(t3 + 0x30) & 0xFF], 116 + SIN_TABLE[(t2 + 0x70) & 0xFF], '0', ((t3 + 0x30 + 0xC0) >> 2) & 0x20, spr_id);
	spr_id = oam_spr(124 + SIN_TABLE[(t3 + 0x40) & 0xFF], 116 + SIN_TABLE[(t2 + 0x80) & 0xFF], '2', ((t3 + 0x40 + 0xC0) >> 2) & 0x20, spr_id);
	spr_id = oam_spr(124 + SIN_TABLE[(t3 + 0x60) & 0xFF], 116 + SIN_TABLE[(t2 + 0xA0) & 0xFF], 'J', ((t3 + 0x60 + 0xC0) >> 2) & 0x20, spr_id);
	spr_id = oam_spr(124 + SIN_TABLE[(t3 + 0x70) & 0xFF], 116 + SIN_TABLE[(t2 + 0xB0) & 0xFF], 'G', ((t3 + 0x70 + 0xC0) >> 2) & 0x20, spr_id);
	spr_id = oam_spr(124 + SIN_TABLE[(t3 + 0x80) & 0xFF], 116 + SIN_TABLE[(t2 + 0xC0) & 0xFF], 'G', ((t3 + 0x80 + 0xC0) >> 2) & 0x20, spr_id);
	oam_hide_rest(spr_id);
	
	if((t0 & 0x3) == 0){
		++c0; if(c0 >= 12) c0 = 0;
		++c1; if(c1 >= 12) c1 = 0;
		++c2; if(c2 >= 12) c2 = 0;
		++c3; if(c3 >= 12) c3 = 0;
		pal_col( 3, 0x11 + c0);
		pal_col( 7, 0x11 + c1);
		pal_col(11, 0x11 + c2);
		pal_col(15, 0x11 + c3);
	}
	
	scroll(0, 240 + (SIN_TABLE[t2] >> 2));
	
	joy = joy_read(0);
	if(JOY_START(joy)){
		sfx_play(0, 0);
		delay(60);
		return text_demo_start();
	}
	
	PPU.mask = mask;
	ppu_wait_nmi();
	return main_demo_loop();
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

TAIL_CALL text_demo_start(){
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
		joy = joy_read(0);
		if(JOY_START(joy)){
			sfx_play(0, 0);
			delay(60);
			return main_demo_start();
		}
		
		ppu_wait_nmi();
	}
}

void main (void) {
	joy_install(joy_static_stddrv);
	
	bank_bg(0);
	bank_spr(1);
	
	ship_demo_start();
}
