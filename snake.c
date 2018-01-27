
#include "snake.h"
#include <stdlib.h>


struct {
    u8 head_x, head_y;
    u8 dirs_available;
    enum {UP, DOWN, LEFT, RIGHT, STILL} state;
} state;


#define SNAKE_HEAD 0x14
#define SNAKE_BODY 0x15
#define MIN_COORD 2
#define MAX_COORD 15

u8 up_buff[32];
u16 up_i = 0;

void set_tile(u8 x, u8 y, u8 c) {
    u16 tile = NTADR_A(x, y);
    
    up_buff[up_i] = (tile>>8)&0xFF;
    up_i++;
    up_buff[up_i] = (tile>>0)&0xFF;
    up_i++;
    up_buff[up_i] = c;
    up_i++;
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
    if(state.head_y<MAX_COORD){
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
    set_tile(2,2,'0'+ship_vx);
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
    set_tile(1,1,'0'+dir);
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
