#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <nes.h>
#include "neslib.h"

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

u8 i, ix, iy = 0;

static const u8 PALETTE[] = {
	0x21, 0x0F, 0x0F, 0x16,
	0x00, 0x0F, 0x0F, 0x2d,
	0x00, 0x0F, 0x0F, 0x2d,
	0x00, 0x0F, 0x0F, 0x2d,
	
	0x00, 0x0F, 0x0F, 0x19,
	0x00, 0x0F, 0x0F, 0x19,
	0x00, 0x0F, 0x0F, 0x19,
	0x00, 0x0F, 0x0F, 0x19,
};

static const char GREET[] = "Global Game Jam 2018!!";

void main (void) {
	ppu_off(); {
		pal_all(PALETTE);
		
		vram_adr(NTADR_A(5, 0));
		for(i = 0; i < sizeof(GREET) - 1; ++i){
			vram_put(GREET[i]);
		}
		
		for(iy = 0; iy < 16; ++iy){
			vram_adr(NTADR_A(8, iy + 8));
			for(ix = 0; ix < 16; ++ix){
				vram_put((iy << 4) | ix);
			}
		}
	} ppu_on_all();
	
	while(true){
		ppu_wait_nmi();
	}
}
