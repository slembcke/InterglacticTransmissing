
#include "snake.h"


struct {
    unsigned char head_x, head_y;
    unsigned char dirs_available;
    enum {UP, DOWN, LEFT, RIGHT, STILL} state;
} state;



void snake_draw_task(void) {
    // TODO actually draw snake
    // May need to rearrange this
    // ppu_off(); {
    //     vram_adr(NTADR_A(state.head_x, state.head_y));
    //     vram_fill(0, 1);
    // } ppu_on_all();
}

void set_state(unsigned int new_state)
{
    state.state = new_state;
}

void find_dirs_avail(void) {
    state.dirs_available = 0;
    if(state.head_y>0){
        state.dirs_available |= (0x1<<UP);
    }
    if(state.head_y<16){
        state.dirs_available |= (0x1<<DOWN);
    }
    if(state.head_x>0){
        state.dirs_available |= (0x1<<LEFT);
    }
    if(state.head_x<16){
        state.dirs_available |= (0x1<<RIGHT);
    }
}


void snake_task(void) {
    find_dirs_avail();
    if(!(state.dirs_available & (0x1<<state.state)))
    {
        set_state(STILL);
    }
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
}

void snake_init(void) {
    state.head_x = 0;
    state.head_y = 0;
    state.state = STILL;
    find_dirs_avail();

    // TODO initial draw    

    snake_draw_task();  

}

void snake_event(unsigned char x, unsigned char y, unsigned char dir) {
    find_dirs_avail();
    // if(x==state.head_x && y==state.head_y) {
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
    // }
}
