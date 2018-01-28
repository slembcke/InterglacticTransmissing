#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"

static const char *TEXT[] = {
	"In amother galaxy. A transm-",
	"issing was sending...",
	"",
	"In case we don't have time",
	"to add more story, use your",
	"imagination please.",
	NULL,
};

TAIL_CALL story_scroller_start(){
	ppu_off(); {
		oam_clear();
		
		vram_inc(0);
		vram_adr(NAMETABLE_A);
		vram_fill(0x00, 32*32);
		
		for(iy = 0; TEXT[iy]; ++iy){
			const char *str = TEXT[iy];
			vram_adr(NTADR_A(2, iy + 2));
			for(ix = 0; str[ix]; ++ix) vram_put(str[ix]);
		}
		
	} ppu_on_all();
	
	// Wait for start clear.
	while(JOY_START(joy_read(0))){}
	
	// Wait for start pressed.
	while(!JOY_START(joy_read(0))){}
	
	return end_level_sequence();
}
