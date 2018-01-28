#ifndef main_h__
#define main_h__

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

extern const u8 MAIN_PALETTE[];
#define ASTEROID_COUNT = 8;
extern const u8 asteroidCornerIndex[];

// Dummy return type to enforce tail calling for gamestates.
typedef struct {} TAIL_CALL;
// Makes the compiler happy.
static TAIL_CALL TERMINATOR(void){return TERMINATOR();}

extern bool TWO_PLAYER;
extern u8 i, ix, iy;
extern u8 spr_id, joy0, joy1;
#pragma zpsym("spr_id")

#define CLAMP(x, min, max) MAX(min, MIN(x, max))

#define SPR_FLIP_X 64
#define SPR_FLIP_Y 128
#define MSPRITE_END 128

typedef struct {
	s16 dx, dy;
	const u8 *msprite;
	
	// 8.8 fixed point.
	u16 x, y;
	s16 vx, vy;
} Ship;

#define TILECOORDS(tile,x,y) tile+(y==1?1:0)+(x==1?0x10:0)

#define DRAWTILE_GRID(x,y,tile) vram_adr(NTADR_A(((x)<<1), ((y)<<1)));\
vram_put(tile);\
vram_adr(NTADR_A(((x)<<1)+1, ((y)<<1)));\
vram_put(tile+1);\
vram_adr(NTADR_A(((x)<<1), ((y)<<1)+1));\
vram_put(tile + 0x10);\
vram_adr(NTADR_A(((x)<<1)+1, ((y)<<1)+1));\
vram_put(tile+0x11);



#define DRAWTILE(x,y,tile) vram_adr(NTADR_A(x, y));\
vram_put(tile);\
vram_adr(NTADR_A(x+ 1, y));\
vram_put(tile + 0x01);\
vram_adr(NTADR_A(x, y + 1));\
vram_put(tile + 0x10);\
vram_adr(NTADR_A(x + 1, y + 1));\
vram_put(tile + 0x11);

extern Ship SHIP[2];

void music_select(u8 song);
TAIL_CALL chr_debug(void);
TAIL_CALL game_loop_start(void);

void ship_init(void);
void ship_update(u8 joy, u8 ship_idx);
void main_event(u8, void * data);

TAIL_CALL end_level_sequence(void);
TAIL_CALL title_loop_start(void);

#endif main_h__