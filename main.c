#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <nes.h>
#include "neslib/neslib.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")

u8 oam_off;

#pragma data-name(pop)
#pragma bss-name (pop)

extern u16 MUSIC;

u8 i, ix, iy;
u8 spr_id;

static const u8 PALETTE[] = {
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

void main (void) {
	ppu_off(); {
		pal_all(PALETTE);
		
		vram_adr(NTADR_A(0, 0));
		vram_fill(128, 32*30);
		
		vram_adr(NTADR_A(4, 3));
		vram_fill(0, sizeof(GREET) + 1);
		vram_adr(NTADR_A(4, 4));
		vram_fill(0, sizeof(GREET) + 1);
		vram_adr(NTADR_A(4, 5));
		vram_fill(0, sizeof(GREET) + 1);
		vram_adr(NTADR_A(5, 4));
		vram_write(GREET, sizeof(GREET) - 1);
		
		
		vram_adr(NTADR_A(0, 30));
		vram_fill(0x50, 8);
		vram_fill(0xFA, 8);
		vram_fill(0x50, 8);
		vram_fill(0xFA, 8);
		vram_fill(0x50, 8);
		vram_fill(0xFA, 8);
		vram_fill(0x50, 8);
		vram_fill(0xFA, 8);
		
		// for(iy = 0; iy < 16; ++iy){
		// 	vram_adr(NTADR_A(12, iy + 8));
		// 	for(ix = 0; ix < 16; ++ix){
		// 		vram_put((iy << 4) | ix);
		// 	}
		// }
	} ppu_on_all();
	
	music_play(0);
	
	while(true){
		static u8 t0, t2, t3, t_4, t_8;
		static u8 c0 = 0, c1 = 3, c2 = 6, c3 = 9;
		
		t0 = nesclock();
		t2 = 2*t0;
		t3 = 3*t0;
		
		spr_id = 0;
		spr_id = oam_spr(128 + SIN_TABLE[(t3 + 0x00) & 0xFF], 120 + SIN_TABLE[(t2 + 0x40) & 0xFF], '!', 0, spr_id);
		spr_id = oam_spr(128 + SIN_TABLE[(t3 + 0x10) & 0xFF], 120 + SIN_TABLE[(t2 + 0x50) & 0xFF], '8', 1, spr_id);
		spr_id = oam_spr(128 + SIN_TABLE[(t3 + 0x20) & 0xFF], 120 + SIN_TABLE[(t2 + 0x60) & 0xFF], '1', 2, spr_id);
		spr_id = oam_spr(128 + SIN_TABLE[(t3 + 0x30) & 0xFF], 120 + SIN_TABLE[(t2 + 0x70) & 0xFF], '0', 3, spr_id);
		spr_id = oam_spr(128 + SIN_TABLE[(t3 + 0x40) & 0xFF], 120 + SIN_TABLE[(t2 + 0x80) & 0xFF], '2', 0, spr_id);
		spr_id = oam_spr(128 + SIN_TABLE[(t3 + 0x60) & 0xFF], 120 + SIN_TABLE[(t2 + 0xA0) & 0xFF], 'J', 1, spr_id);
		spr_id = oam_spr(128 + SIN_TABLE[(t3 + 0x70) & 0xFF], 120 + SIN_TABLE[(t2 + 0xB0) & 0xFF], 'G', 2, spr_id);
		spr_id = oam_spr(128 + SIN_TABLE[(t3 + 0x80) & 0xFF], 120 + SIN_TABLE[(t2 + 0xC0) & 0xFF], 'G', 3, spr_id);
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
		
		ppu_wait_nmi();
	}
}
