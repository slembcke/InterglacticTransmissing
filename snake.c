
#include "snake.h"
#include <stdlib.h>


struct {
    u8 head_x, head_y;
    u8 dirs_available;
    enum {UP, DOWN, LEFT, RIGHT, STILL} state;
} state;


#define SNAKE_HEAD 0x14
#define SNAKE_BODY 0x15
#define MIN_COORD 1
#define MAX_COORD 14

#define BIG_TILE_UPDATE_SIZE 10
#define BIG_TILE_MAX_COUNT 4

u8 up_buff[BIG_TILE_UPDATE_SIZE*BIG_TILE_MAX_COUNT];
u8 up_i = 0;

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
    (up_buff + 3)[up_i] = c;
    (up_buff + 4)[up_i] = c;

    (up_buff + 5)[up_i] = NT_UPD_HORZ | (tile_lo>>8)&0xFF;
    (up_buff + 6)[up_i] = (tile_lo>>0)&0xFF;
    (up_buff + 7)[up_i] = 2;
    (up_buff + 8)[up_i] = c;
    (up_buff + 9)[up_i] = c;

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
    state.dirs_available = 0;
    if(state.head_y>MIN_COORD){
        state.dirs_available |= (0x1<<UP);
    }
    if(state.head_y<MAX_COORD-1){
        state.dirs_available |= (0x1<<DOWN);
    }
    if(state.head_x>MIN_COORD){
        state.dirs_available |= (0x1<<LEFT);
    }
    if(state.head_x<MAX_COORD){
        state.dirs_available |= (0x1<<RIGHT);
    }
}


void snake_task(void) {
    find_dirs_avail();
    if(!(state.dirs_available & (0x1<<state.state)))
    {
        set_state(STILL);
    }
    else {
        set_tile(state.head_x, state.head_y, '#');
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
        set_tile(state.head_x, state.head_y, '^');
    }
}

void snake_init(void) {
    state.head_x = 8;
    state.head_y = 8;
    state.state = STILL;
    find_dirs_avail();

    clear_up();
    set_tile(state.head_x, state.head_y, '^');
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

    find_dirs_avail();
    ship_y = ship_y_raw/8;
    ship_x = ship_x_raw/8;
    dir = get_dir(ship_vx, ship_vy);
    if(ship_x==state.head_x && ship_y==state.head_y) {
            set_tile(ship_x, ship_y, '*');
        if((0x1<<dir)&state.dirs_available) {
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
