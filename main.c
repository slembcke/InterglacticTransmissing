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
	0x21, 0x0F, 0x0F, 0x20,
	0x00, 0x0F, 0x0F, 0x2C,
	0x00, 0x0F, 0x0F, 0x2C,
	0x00, 0x0F, 0x0F, 0x2C,
	
	0x00, 0x00, 0x00, 0x36,
	0x00, 0x00, 0x00, 0x37,
	0x00, 0x00, 0x00, 0x38,
	0x00, 0x00, 0x00, 0x39,
};

static const char GREET[] = "Global Game Jam 2018!!";

static const s8 SIN_TABLE[] = {
	0, 1, 3, 4, 6, 7, 8, 10, 11, 12, 13, 14, 14, 15, 15, 15, 16, 15,
	15, 15, 14, 14, 13, 12, 11, 10, 8, 7, 6, 4, 3, 1, 0, -1, -3, -4,
	-6, -7, -8, -10, -11, -12, -13, -14, -14, -15, -15, -15, -16, -15,
	-15, -15, -14, -14, -13, -12, -11, -10, -8, -7, -6, -4, -3, -1,
};

void main (void) {
	ppu_off(); {
		pal_all(PALETTE);
		
		vram_adr(NTADR_A(5, 4));
		for(i = 0; i < sizeof(GREET) - 1; ++i){
			vram_put(GREET[i]);
		}
		
		for(iy = 0; iy < 16; ++iy){
			vram_adr(NTADR_A(8, iy + 8));
			for(ix = 0; ix < 16; ++ix){
				vram_put((iy << 4) | ix);
			}
		}
		
		vram_adr(NTADR_A(0, 30));
		for(i = 0; i < 16; ++i){
			vram_put(0xFF);
		}
	} ppu_on_all();
	
	music_play(0);
	
	while(true){
		static u8 t0, t1, t2, t3;
		
		t0 = nesclock();
		t1 = t0 >> 1;
		t2 = t1 >> 1;
		t3 = t2 >> 1;
		
		spr_id = 0;
		spr_id = oam_spr(32 + SIN_TABLE[(t0 +  0) & 0x3F], 120 + SIN_TABLE[(t0 + 16) & 0x3F], '!', (t2 + 0) & 0x03, spr_id);
		spr_id = oam_spr(32 + SIN_TABLE[(t0 +  8) & 0x3F], 120 + SIN_TABLE[(t0 + 19) & 0x3F], '8', (t2 + 1) & 0x03, spr_id);
		spr_id = oam_spr(32 + SIN_TABLE[(t0 + 16) & 0x3F], 120 + SIN_TABLE[(t0 + 22) & 0x3F], '1', (t2 + 2) & 0x03, spr_id);
		spr_id = oam_spr(32 + SIN_TABLE[(t0 + 24) & 0x3F], 120 + SIN_TABLE[(t0 + 25) & 0x3F], '0', (t2 + 3) & 0x03, spr_id);
		spr_id = oam_spr(32 + SIN_TABLE[(t0 + 32) & 0x3F], 120 + SIN_TABLE[(t0 + 28) & 0x3F], '2', (t2 + 0) & 0x03, spr_id);
		spr_id = oam_spr(32 + SIN_TABLE[(t0 + 40) & 0x3F], 120 + SIN_TABLE[(t0 + 31) & 0x3F], 'J', (t2 + 1) & 0x03, spr_id);
		spr_id = oam_spr(32 + SIN_TABLE[(t0 + 48) & 0x3F], 120 + SIN_TABLE[(t0 + 34) & 0x3F], 'G', (t2 + 2) & 0x03, spr_id);
		spr_id = oam_spr(32 + SIN_TABLE[(t0 + 56) & 0x3F], 120 + SIN_TABLE[(t0 + 37) & 0x3F], 'G', (t2 + 3) & 0x03, spr_id);
		oam_hide_rest(spr_id);
		
		// sfx_play(0, 0);
		
		pal_col(0, 0x01 + (t3 & 0x07));
		
		ppu_wait_nmi();
	}
}
