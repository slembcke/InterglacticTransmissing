#include "main.h";
#include "neslib/neslib.h";


// #include "red_ship.h"
// #include "blue_ship.h"
// #include "tri_ship.h"
#include "ang_ship.h"

static u8 tick=0;


#define ENEMY_PALETTE 2
#define PIXELS_PER_COORD (16)
#define CHUNK (2)


struct  {
    u8 x, y;
    const u8 *msprite;
} enemy;

struct {
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
} bourder_route;

void enemy_init(void) {
    enemy.x=bourder_route.left;
    enemy.y=bourder_route.top;
    enemy.msprite = ANG_DIRECTIONS[0];

    bourder_route.top=3;
    bourder_route.bottom=13;
    bourder_route.left=3;
    bourder_route.right=13;
}


enum {GO_LEFT, GO_UP, GO_RIGHT, GO_DOWN} go_state = GO_RIGHT;

void enemy_move(void) {
    if(go_state==GO_RIGHT) {if(++enemy.x==bourder_route.right) {go_state=GO_DOWN;}} else 
    if(go_state==GO_DOWN)   {if(++enemy.y==bourder_route.bottom) {go_state=GO_LEFT;}} else 
    if(go_state==GO_LEFT){if(--enemy.x==bourder_route.left)  {go_state=GO_UP;}} else
    if(go_state==GO_UP) {if(--enemy.y==bourder_route.top)  {go_state=GO_RIGHT;}}
}

void enemy_update(void){
    static const u8 *msprite;
    static u8 tick=0;
    static u8 pixels=0;
    s8 x_delta=0, y_delta=0;

    if(go_state==GO_RIGHT) {x_delta++;} else 
    if(go_state==GO_DOWN)  {y_delta++;} else 
    if(go_state==GO_LEFT)  {x_delta--;} else
    if(go_state==GO_UP)    {y_delta--;}
    if(tick%CHUNK==0) {
        pixels+=CHUNK;
        oam_meta_spr_pal((enemy.x*PIXELS_PER_COORD)+x_delta, (enemy.y*PIXELS_PER_COORD)+y_delta, ENEMY_PALETTE, enemy.msprite);
    }
    if((pixels)%PIXELS_PER_COORD==0) {    
        enemy_move();
        tick=0;
    }

    msprite = ANG_DIRECTIONS[go_state];
    if(msprite) enemy.msprite = msprite;
    
    // if(joy & 0xF0){
    //     register fx = (ship.x >> 8) + FX[joy >> 4], fy = (ship.y >> 8) + FY[joy >> 4];
    //     spr_id = oam_spr(fx, fy, 0xCC + (fire & 0x3), ((fire & 0xC) << 4) | (!ship_idx), spr_id);
    //     ++fire;
    // }
}
