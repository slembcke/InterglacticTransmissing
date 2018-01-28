#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

#include "main.h"

static const char *TEXT[] = {
	" Interglactic Transmissing!",
	"", "",
	"      Art:",
	"        Bill Nagel",
	"", "",
	"      Music:",
	"        Troy Strand",
	"", "",
	"      Programming:",
	"        Andy Korth",
	"        Ben Gartner",
	"        Scott Lembcke",
	"",
	NULL,
};

TAIL_CALL credits_start(){
	fade_out();
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
		
		// Make some lines blue.
		vram_adr(NTADR_A(0*8, 30));
		vram_fill(0x50, 8);
		vram_fill(0x05, 8);
		vram_fill(0x05, 8);
		vram_fill(0x05, 8);
		
	} ppu_on_all();
	fade_in();
	
	// Wait for start clear.
	while(JOY_START(joy_read(0))){}
	
	while(true){
		if(JOY_START(joy_read(0))) break;
		
		spr_id = 0;
		draw_title_stars();
		oam_hide_rest(spr_id);
		
		ppu_wait_nmi();
	}
	
	// Wait for start clear.
	while(JOY_START(joy_read(0))){}
	
	return title_loop_start();
}
