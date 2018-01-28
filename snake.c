#include <stdlib.h>

#include "snake.h"
#include "levels.h"

#define THROTTLE (4)

struct {
    u16 signal_x, signal_y;
    u8 sig_str;
    u8 throttle_ctr;
    u8 dirs_available;
    enum {UP=EVENT_UP, DOWN=EVENT_DW, LEFT=EVENT_LF, RIGHT=EVENT_RT, STILL} state;
    u32 sig_dir;
} state;


#define SNAKE_HEAD 0x14
#define SNAKE_BODY 0x15
#define MIN_COORD 1
#define MAX_COORD 14
#define SIG_DELTA 0x20

#define BIG_TILE_UPDATE_SIZE 10
#define BIG_TILE_MAX_COUNT 4
#define POW(x) (0x0001 << (x))

#define OFFX 4
#define OFFY 3

static const u8 SIGNAL_UP_MSPRITE[] = {
     0,  0, 0x8C, 0,
     8,  0, 0x8D, 0,
     0,  8, 0x9C, 0,
     8,  8, 0x9D, 0,
    MSPRITE_END
};

static const u8 SIGNAL_DOWN_MSPRITE[] = {
     0,  0, 0x9C, SPR_FLIP_Y,
     8,  0, 0x9D, SPR_FLIP_Y,
     0,  8, 0x8C, SPR_FLIP_Y,
     8,  8, 0x8D, SPR_FLIP_Y,
    MSPRITE_END
};

static const u8 SIGNAL_RIGHT_MSPRITE[] = {
     0,  0, 0xAC, 0,
     8,  0, 0xAD, 0,
     0,  8, 0xBC, 0,
     8,  8, 0xBD, 0,
    MSPRITE_END
};

static const u8 SIGNAL_LEFT_MSPRITE[] = {
     0,  0, 0xAC, SPR_FLIP_X,
     8,  0, 0xAD, SPR_FLIP_X,
     0,  8, 0xBC, SPR_FLIP_X,
     8,  8, 0xBD, SPR_FLIP_X,
    MSPRITE_END
};

static const u8 * const SIGNAL_DIRECTIONS[] = {
    SIGNAL_UP_MSPRITE,
    SIGNAL_DOWN_MSPRITE,
    SIGNAL_LEFT_MSPRITE,
    SIGNAL_RIGHT_MSPRITE,
};


u8 up_buff[BIG_TILE_UPDATE_SIZE*BIG_TILE_MAX_COUNT];
u8 up_i = 0;
u8 block_lu[2][2];
u16 collision_map[16];
const u16 pow2[16] = {
    POW(0), POW(1), POW(2), POW(3), POW(4), POW(5), POW(6), POW(7),
    POW(8), POW(9), POW(10), POW(11), POW(12), POW(13), POW(14), POW(15)
};
u8 parse_tile(u8 i, u8 j, u8 c) {
    return c;
}

void set_tile(u8 x, u8 y, u8 c) {
    u16 tile_hi;
    u16 tile_lo;

    if (up_i > (BIG_TILE_UPDATE_SIZE-1)*BIG_TILE_MAX_COUNT) {
        up_i=0;
        set_tile(0,0,'!');
        return;
    }
    tile_hi = NTADR_A(x<<1, y<<1);
    tile_lo = NTADR_A(x<<1, (y<<1)+1);
    
    (up_buff + 0)[up_i] = NT_UPD_HORZ | (tile_hi>>8)&0xFF;
    (up_buff + 1)[up_i] = (tile_hi>>0)&0xFF;
    (up_buff + 2)[up_i] = 2;
    (up_buff + 3)[up_i] = TILECOORDS(c,0,0);
    (up_buff + 4)[up_i] = TILECOORDS(c,0,1);

    (up_buff + 5)[up_i] = NT_UPD_HORZ | (tile_lo>>8)&0xFF;
    (up_buff + 6)[up_i] = (tile_lo>>0)&0xFF;
    (up_buff + 7)[up_i] = 2;
    (up_buff + 8)[up_i] = TILECOORDS(c,1,0);
    (up_buff + 9)[up_i] = TILECOORDS(c,1,1);

    up_i+= BIG_TILE_UPDATE_SIZE;

    up_buff[up_i] = NT_UPD_EOF;
}

void clear_up(void) {
    up_buff[0] = NT_UPD_EOF;
    up_i=0;
}

void snake_draw_post(void) {
    clear_up();
}

void set_state(u32 new_state)
{
    state.state = new_state;
}

void find_dirs_avail(void) {
    u16 row=0;
    u16 mask, left, right, above, below;

    row = collision_map[state.signal_y >> 12];
    mask = pow2[state.signal_x >> 12];
    left = mask >> 1;
    left &= row;
    right = mask << 1;
    right &= row;
    above = (collision_map-1)[state.signal_y >> 12];
    below = (collision_map+1)[state.signal_y >> 12];
    
    state.dirs_available = 0;
    
    if((above&mask)==0){
        state.dirs_available |= (pow2[EVENT_UP]);
    }
    if((below&mask)==0){
        state.dirs_available |= (pow2[EVENT_DW]);
    }
    if(left==0){
        state.dirs_available |= (pow2[EVENT_LF]);
    }
    if(right==0){
        state.dirs_available |= (pow2[EVENT_RT]);
    }
}

const char HEX[] = "0123456789ABCDEF";

bool is_start_position(void) {
    return((state.signal_x>>12) == level_0_start[0]+OFFX && 
        (state.signal_y>>12) ==level_0_start[1]+OFFY);
}

void snake_task(void) {
    spr_id = oam_meta_spr((state.signal_x>>8), (state.signal_y>>8), spr_id, SIGNAL_DIRECTIONS[state.sig_dir]);
    if(!is_start_position()) {
        if((state.sig_str > '0') && 0==(state.throttle_ctr%64)) {
            state.sig_str -= 1;
            //TODO //Represent signal strength
            set_tile(0, 0, state.sig_str); //~ radiowave
        }
    }
    state.throttle_ctr += 1;
    if((state.state < STILL) && !(state.dirs_available & pow2[state.state]))
    {
        set_state(STILL);
    }
    else {
        // if(0==(state.throttle_ctr%THROTTLE)) 
        {
            if(!is_start_position()) {
                set_tile(state.signal_x>>12, state.signal_y>>12, 0xA6); //0xA6=spacedust
            }
            collision_map[state.signal_y>>12] |= pow2[state.signal_x>>12];
            if(state.state==DOWN) {
                state.signal_y += SIG_DELTA;
                state.sig_dir = state.state;
            }
            if(state.state==UP) {
                state.signal_y -= SIG_DELTA;
                state.sig_dir = state.state;
            }
            if(state.state==RIGHT) {
                state.signal_x += SIG_DELTA;
                state.sig_dir = state.state;
            }
            if(state.state==LEFT) {
                state.signal_x -= SIG_DELTA;
                state.sig_dir = state.state;
            }
            find_dirs_avail();
            state.sig_str = '8';
        }
    }
}



void snake_init(void) {
    u8 i=0;
    u16 row=0x01, mask=0;
    u8 x=0,y=0;
    collision_map[0] = 0xFFFF; //map border
    collision_map[1] = 0xFFFF; //map border
    collision_map[2] = 0xFFFF; //map border
    collision_map[11] = 0xFFFF; //map border
    collision_map[12] = 0xFFFF; //map border
    collision_map[13] = 0xFFFF; //map border
    collision_map[14] = 0xFFFF; //map border
    collision_map[15] = 0xFFFF; //map border
    for(i=0;i<8;i++) {
        collision_map[i+3] = level_0[i];
    }

    for(x=0;x<16;x++) {
        mask = pow2[x];
        for(y=0;y<15;y++) {
            if( (collision_map[y]&mask)==mask) {
                DRAWTILE(x*2,y*2, asteroidCornerIndex[(x + (y << 1) ) % 8] );
            }
        }
    }
    vram_adr(NTADR_A(0, 30) );
        
    for(y=0;y<8;y++) {
        u8 value;
        for(x=0;x<8;x++) {
            u16 maskR = pow2[x * 2 + 1]; // TODO: should shift by this amount instead of masking with a 16 bit number.
            u16 maskL = pow2[x * 2];
            // collect all 4 tiles:
            // palette 0x01 is the asteroid palette 
            
            value = ((collision_map[y * 2]&maskL ) == maskL)  
                  | ((collision_map[y * 2]&maskR) == maskR) << 2 
                  | ((collision_map[y * 2 + 1]&maskL) == maskL) << 4 
                  | ((collision_map[y * 2 + 1]&maskR) == maskR) << 6;
            vram_put(value << 1);  // 10 10 10 10
        }
    }

 
    state.sig_dir = UP;
    x=level_0_start[0]+OFFX;
    y=level_0_start[1]+OFFY;
    state.signal_x = (x<<(8+4));
    state.signal_y = (y<<(8+4));
    DRAWTILE_GRID(x,y, 0xAA); //0xAA satelite
    x=level_0_end[0]+OFFX;
    y=level_0_end[1]+OFFY;
    DRAWTILE_GRID(x,y, 0xAA); //0xAA satelite
    state.throttle_ctr = 0;
    state.sig_str = '8';
    state.state = STILL;

    find_dirs_avail();
    set_vram_update(up_buff);
}

u8 get_dir(s16 ship_vx, s16 ship_vy) {
    u16 mag_x, mag_y;
    mag_x = abs(ship_vx);
    mag_y = abs(ship_vy);
    if(ship_vx >=0 && mag_x >= mag_y) {
        return RIGHT;
    }
    else if(ship_vx <=0 && mag_x >= mag_y) {
        return LEFT;
    }
    else if(ship_vy >=0 && mag_y >= mag_x) {
        return DOWN;
    }
    else if(ship_vy <=0 && mag_y >= mag_x) {
        return UP;
    }
}

void snake_event(u16 ship_x, u16 ship_y, s16 ship_vx, s16 ship_vy)
{
    u8 dir;

    dir = get_dir(ship_vx, ship_vy);
    if((ship_x>>12)==(state.signal_x>>12) && (ship_y>>12)==(state.signal_y>>12)) {
        if((pow2[dir])&state.dirs_available) {
            if(dir==EVENT_DW){
                set_state(DOWN);
            }
            if(dir==EVENT_UP){
                set_state(UP);
            }
            if(dir==EVENT_LF){
                set_state(LEFT);
            }
            if(dir==EVENT_RT){
                set_state(RIGHT);
            }
        }
    }
}
