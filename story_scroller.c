#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"
#include <string.h>

#include "main.h"

static const char *TEXT[] = {
	"In amother galaxy. A transm-",
	"issing was sending...",
	"",
	"Critical data is stranded.",
	"Your mission is to keep the",
	"signal alive.",
	"",
	"Asteriods and Garfon IX",
	"voidcrafts block progress",
	"",
	"If the signal fades, you",
	"Will fail.",
	"",
	"That's how you lose this game",
	"",
	"No one will",
	"hold failure against you.",
	"",
	"That's just not the culture",
	"here.",
	"",
	"But still.",
	NULL,
};

TAIL_CALL story_scroller_start(){
	fade_out();
	ppu_off(); {
	 	oam_clear();
		vram_inc(0);
		vram_adr(NAMETABLE_A);
		vram_fill(0x00, 32*32);
		fade_in();

	} ppu_on_all();

	for(iy = 0; TEXT[iy]; ++iy){
		const char *str = TEXT[iy];
		ppu_off(); {
			
			vram_adr(NTADR_A(2, iy + 2));
			for(ix = 0; str[ix]; ++ix) vram_put(str[ix]);
			
		} ppu_on_all();
		if(0==strlen(TEXT[iy])) {
			// Wait for start clear.
			while(JOY_START(joy_read(0))){}
			
			// Wait for start pressed.
			while(!JOY_START(joy_read(0))){}
		}

	}
	
	// Wait for start clear.
	while(JOY_START(joy_read(0))){}
	
	// Wait for start pressed.
	while(!JOY_START(joy_read(0))){}
	
	return end_level_sequence();
}
