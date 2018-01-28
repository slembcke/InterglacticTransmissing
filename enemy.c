#include "main.h";
#include "neslib/neslib.h";


// #include "red_ship.h"
// #include "blue_ship.h"
// #include "tri_ship.h"
#include "ang_ship.h"
#include "enemy.h"

static u8 tick=0;


#define ENEMY_PALETTE 2
#define PIXELS_PER_COORD (16)
#define CHUNK (2)


struct  {
    u16 x, y;
    const u8 *msprite;
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
} enemy;


void enemy_init(void) {
    enemy.top=3;
    enemy.bottom=13;
    enemy.left=3;
    enemy.right=13;
    enemy.x=enemy.left<<4;
    enemy.y=enemy.top<<4;
    enemy.msprite = ANG_DIRECTIONS[0];
    oam_meta_spr_pal(enemy.x, enemy.y, ENEMY_PALETTE, enemy.msprite);
}


enum {GO_LEFT, GO_UP, GO_RIGHT, GO_DOWN} go_state = GO_RIGHT;

void enemy_move(void) {
    if(go_state==GO_RIGHT) {if((++enemy.x)>>4>=enemy.right) {go_state=GO_DOWN;}} else 
    if(go_state==GO_DOWN)   {if((++enemy.y)>>4>=enemy.bottom) {go_state=GO_LEFT;}} else 
    if(go_state==GO_LEFT){if((--enemy.x)>>4<=enemy.left)  {go_state=GO_UP;}} else
    if(go_state==GO_UP) {if((--enemy.y)>>4<=enemy.top)  {go_state=GO_RIGHT;}}
}

void enemy_update(void){
    static const u8 *msprite;
    static u8 tick=0;
    static u8 pixels=0;

    enemy_move();
    oam_meta_spr_pal(enemy.x, enemy.y, ENEMY_PALETTE, enemy.msprite);

    msprite = ANG_DIRECTIONS[go_state];
    if(msprite) enemy.msprite = msprite;
    
    // if(joy & 0xF0){
    //     register fx = (ship.x >> 8) + FX[joy >> 4], fy = (ship.y >> 8) + FY[joy >> 4];
    //     spr_id = oam_spr(fx, fy, 0xCC + (fire & 0x3), ((fire & 0xC) << 4) | (!ship_idx), spr_id);
    //     ++fire;
    // }
}

enemy_status_t enemy_event(u8 ship_x, u8 ship_y)
{

    if(
        enemy.x - 8 <= ship_x && ship_x <= enemy.x + 8 &&
        enemy.y - 8 <= ship_y && ship_y <= enemy.y + 8
    )
    {
        return ENEMY_LOSS;
    }
    return NOT_DEAD_YET;
}

