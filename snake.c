#include <stdlib.h>

#include "snake.h"
#include "levels.h"

#define THROTTLE (4)

struct {
    u8 head_x, head_y;
    u8 throttle_ctr;
    u8 dirs_available;
    enum {UP=EVENT_UP, DOWN=EVENT_DW, LEFT=EVENT_LF, RIGHT=EVENT_RT, STILL} state;
} state;


#define SNAKE_HEAD 0x14
#define SNAKE_BODY 0x15
#define MIN_COORD 1
#define MAX_COORD 14

#define BIG_TILE_UPDATE_SIZE 10
#define BIG_TILE_MAX_COUNT 4
#define POW(x) (0x0001 << (x))

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

void snake_draw_task(void) {
    // TODO actually draw snake
    // May need to rearrange this
    // ppu_off(); {
    //     vram_adr(NTADR_A(state.head_x, state.head_y));
    //     // vram_adr(NTADR_A(7, 7));
    //     vram_put(0x14);
        
    // } ppu_on_all();
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

    row = collision_map[state.head_y];
    mask = pow2[state.head_x];
    left = mask >> 1;
    left &= row;
    right = mask << 1;
    right &= row;
    above = (collision_map-1)[state.head_y];
    below = (collision_map+1)[state.head_y];
    
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

void snake_task(void) {

    if(!(state.dirs_available & pow2[state.state]))
    {
        set_state(STILL);
        state.throttle_ctr=0;
    }
    else {
        state.throttle_ctr += 1;
        if(THROTTLE==state.throttle_ctr) {
            state.throttle_ctr = 0;
            set_tile(state.head_x, state.head_y, 0xA6); //0xA6=spacedust
            collision_map[state.head_y] |= pow2[state.head_x];
            if(state.state==DOWN) {
                state.head_y += 1;
            }
            if(state.state==UP) {
                state.head_y -= 1;
            }
            if(state.state==RIGHT) {
                state.head_x += 1;
            }
            if(state.state==LEFT) {
                state.head_x -= 1;
            }
            find_dirs_avail();
            set_tile(state.head_x, state.head_y, 0xAA); //0xAA=satelite            
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
        for(y=0;y<15;y++) {
            mask = pow2[x];
            if((collision_map[y]&mask)==mask) {
                DRAWTILE(x,y,0x80); //0x80 = asteroid
            }
        }
    }
    state.head_x = level_0_x + 4;
    state.head_y = level_0_y + 3;
    state.throttle_ctr = 0;
    state.state = STILL;
    find_dirs_avail();

    clear_up();
    set_tile(state.head_x, state.head_y, 0xAA); //0xAA satelite
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

void snake_event(u8 ship_x_raw, u8 ship_y_raw, s16 ship_vx, s16 ship_vy)
{
    u8 ship_x;
    u8 ship_y;
    u8 dir;

    ship_y = ship_y_raw/8;
    ship_x = ship_x_raw/8;
    dir = get_dir(ship_vx, ship_vy);
    if(ship_x==state.head_x && ship_y==state.head_y) {
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
