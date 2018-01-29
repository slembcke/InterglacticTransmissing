#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"
#include <string.h>

#include "main.h"

static void write_str(const char *str){
	vram_write(str, strlen(str));
}

TAIL_CALL level_intro_start(void){
	static u8 digit1;
	static u8 digit10;
	
	digit1 = (CURRENT_LEVEL + 1)%10;
	digit10 = (CURRENT_LEVEL + 1)/10;
	
	fade_out();
	ppu_off(); {
	 	oam_clear();
		vram_inc(0);
		vram_adr(NAMETABLE_A);
		vram_fill(0x00, 32*32);
		
		vram_adr(NTADR_A(9, 12));
		write_str("LEVEL ");
		vram_put('0' + digit1);
		if(digit10) vram_put('0' + digit10);
		write_str(" START!");
	} ppu_on_all();
	fade_in();
	
	delay(2*60);
	
	return game_loop_start();
}
