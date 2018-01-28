#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"
#include <string.h>

#include "main.h"

static const char *TEXT[] = {
	"20XX: amother galaxy, A tra-",
	"nsmissing was sending...",
	"",
	"  Critical data is stranded.",
	"Your mission is to keep the",
	"signal alive.",
	"",
	"  As Captain Major Job", 
	"Bohnson it is your duty to",
	"ensure the transmissing",
	"arrives to full sector",
	"with time...",
	"",
	"  No one will",
	"hold failure against you...",
	"",
	"Hold B to signal boost.",
	NULL,
};

TAIL_CALL story_scroller_start(){
	fade_out();
	ppu_off(); {
	 	oam_clear();
		vram_inc(0);
		vram_adr(NAMETABLE_A);
		vram_fill(0x00, 32*32);
		vram_adr(NAMETABLE_C);
		vram_fill(0x00, 32*32);

		for(iy = 0; TEXT[iy]; ++iy){
			const char *str = TEXT[iy];
			vram_adr(NTADR_C(2, iy + 2));
			for(ix = 0; str[ix]; ++ix) vram_put(str[ix]);
		}
	} ppu_on_all();
	fade_in();
	
	iy = 0;
	scroll(0, iy);
	
	// Wait for start clear.
	while(JOY_START(joy_read(0))){}
	
	// Wait for start pressed.
	while(!JOY_START(joy_read(0))){
		scroll(0, iy);
		if(iy < 200 && nesclock() & 1) ++iy;
		
		spr_id = 0;
		draw_title_stars();
		oam_hide_rest(spr_id);
		
		ppu_wait_nmi();
	}
	
	scroll(0, 0);
	
	return end_level_sequence();
}
