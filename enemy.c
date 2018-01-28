#include "main.h";
#include "neslib/neslib.h";


#include "red_ship.h"
#include "blue_ship.h"
#include "tri_ship.h"
#include "ang_ship.h"
#include "enemy.h"

static u8 tick=0;


#define ENEMY_PALETTE 2
#define PIXELS_PER_COORD (16)
#define CHUNK (2)

typedef enum go_state {GO_LEFT, GO_UP, GO_RIGHT, GO_DOWN, GO_LEFT_UP, GO_RIGHT_UP, GO_RIGHT_DOWN, GO_LEFT_DOWN} go_state_t;
u8 bogie_count;
#define MAX_BOGIES (4)

#define CUT (4)

struct  {
    u16 x, y;
    const u8 *msprite;
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
    go_state_t go;
    enum {CLOCK, OTHER, OCTOGON, PATROL, RANDOM, ABSENT} rotation;
    const u8 * const * directions;
} enemy[MAX_BOGIES];


void enemy_init(void) {
    bogie_count=CURRENT_LEVEL;

    if(CURRENT_LEVEL<=2) {
        enemy[0].top=0;
        enemy[0].bottom=16;
        enemy[0].left=3;
        enemy[0].right=13;
        enemy[0].x=8<<4;
        enemy[0].y=8<<4;
        enemy[0].msprite = ANG_DIRECTIONS[0];
        enemy[0].go = GO_UP;
        enemy[0].rotation = PATROL;
        enemy[0].directions = ANG_DIRECTIONS;

        enemy[1].top=0;
        enemy[1].bottom=16;
        enemy[1].left=0;
        enemy[1].right=16;
        enemy[1].x=8<<4;
        enemy[1].y=8<<4;
        enemy[1].msprite = BLUE_DIRECTIONS[1];
        enemy[1].go = GO_LEFT;
        enemy[1].rotation = PATROL;
        enemy[1].directions = ANG_DIRECTIONS;
    } else
    {
        bogie_count=MAX_BOGIES;
        if(CURRENT_LEVEL-2 < MAX_BOGIES)
            bogie_count=CURRENT_LEVEL-2;

        enemy[0].top=3;
        enemy[0].bottom=13;
        enemy[0].left=3;
        enemy[0].right=13;
        enemy[0].x=enemy[0].left<<4;
        enemy[0].y=enemy[0].top<<4;
        enemy[0].msprite = ANG_DIRECTIONS[0];
        enemy[0].go = GO_RIGHT;
        enemy[0].rotation = CLOCK;
        enemy[0].directions = ANG_DIRECTIONS;

        enemy[1].top=5;
        enemy[1].bottom=11;
        enemy[1].left=5;
        enemy[1].right=11;
        enemy[1].x=enemy[1].left<<4;
        enemy[1].y=enemy[1].top<<4;
        enemy[1].msprite = TRI_DIRECTIONS[0];
        enemy[1].go = GO_LEFT;
        enemy[1].rotation = OTHER;
        enemy[1].directions = TRI_DIRECTIONS;

        enemy[2].top=1;
        enemy[2].bottom=14;
        enemy[2].left=1;
        enemy[2].right=14;
        enemy[2].x=enemy[2].left<<4;
        enemy[2].y=enemy[2].top<<4;
        enemy[2].msprite = BLUE_DIRECTIONS[0];
        enemy[2].go = GO_LEFT;
        enemy[2].rotation = OCTOGON;
        enemy[2].directions = BLUE_DIRECTIONS;

        enemy[3].top=1;
        enemy[3].bottom=14;
        enemy[3].left=1;
        enemy[3].right=14;
        enemy[3].x=8<<4;
        enemy[3].y=8<<4;
        enemy[3].msprite = RED_DIRECTIONS[0];
        enemy[3].go = GO_LEFT;
        enemy[3].rotation = PATROL;
        enemy[3].directions = RED_DIRECTIONS;        
    }
}


void enemy_move(u8 i) {
    if(enemy[i].rotation==CLOCK) {
        if(enemy[i].go==GO_RIGHT) {if((++enemy[i].x)>>4>=enemy[i].right)  {enemy[i].go=GO_DOWN;}} else 
        if(enemy[i].go==GO_DOWN)  {if((++enemy[i].y)>>4>=enemy[i].bottom) {enemy[i].go=GO_LEFT;}} else 
        if(enemy[i].go==GO_LEFT)  {if((--enemy[i].x)>>4<=enemy[i].left)   {enemy[i].go=GO_UP;}} else
        if(enemy[i].go==GO_UP)    {if((--enemy[i].y)>>4<=enemy[i].top)    {enemy[i].go=GO_RIGHT;}}        
    }
    if(enemy[i].rotation==OTHER) {
        if(enemy[i].go==GO_RIGHT) {if((++enemy[i].x)>>4>=enemy[i].right)  {enemy[i].go=GO_UP;}} else 
        if(enemy[i].go==GO_DOWN)  {if((++enemy[i].y)>>4>=enemy[i].bottom) {enemy[i].go=GO_RIGHT;}} else 
        if(enemy[i].go==GO_LEFT)  {if((--enemy[i].x)>>4<=enemy[i].left)   {enemy[i].go=GO_DOWN;}} else
        if(enemy[i].go==GO_UP)    {if((--enemy[i].y)>>4<=enemy[i].top)    {enemy[i].go=GO_LEFT;}}        
    }
    if(enemy[i].rotation==OCTOGON) {
        if(enemy[i].go==GO_RIGHT)      {if((++enemy[i].x)>>4>=enemy[i].right-CUT) {enemy[i].go=GO_RIGHT_DOWN;}} else 
        if(enemy[i].go==GO_RIGHT_DOWN) {    ++enemy[i].x;   ++enemy[i].y;
                                        if(  (enemy[i].x)>>4>=enemy[i].right)     {enemy[i].go=GO_DOWN;}} else 
        if(enemy[i].go==GO_DOWN)       {if((++enemy[i].y)>>4>=enemy[i].bottom-CUT){enemy[i].go=GO_LEFT_DOWN;}} else 
        if(enemy[i].go==GO_LEFT_DOWN)  {    ++enemy[i].y;   --enemy[i].x;
                                        if((  enemy[i].y)>>4>=enemy[i].bottom)    {enemy[i].go=GO_LEFT;}} else 
        if(enemy[i].go==GO_LEFT)       {if((--enemy[i].x)>>4<=enemy[i].left+CUT)  {enemy[i].go=GO_LEFT_UP;}} else
        if(enemy[i].go==GO_LEFT_UP)    {    --enemy[i].x;   --enemy[i].y;
                                        if((  enemy[i].x)>>4<=enemy[i].left)      {enemy[i].go=GO_UP;}} else
        if(enemy[i].go==GO_UP)         {if((--enemy[i].y)>>4<=enemy[i].top+CUT)   {enemy[i].go=GO_RIGHT_UP;}}        
        if(enemy[i].go==GO_RIGHT_UP)   {    --enemy[i].y;   ++enemy[i].x;
                                        if((  enemy[i].y)>>4<=enemy[i].top)       {enemy[i].go=GO_RIGHT;}}       
    } 
    if(enemy[i].rotation==PATROL) {
        if(enemy[i].go==GO_RIGHT) {if((++enemy[i].x)>>4>=enemy[i].right)  {enemy[i].go=GO_LEFT;}} else 
        if(enemy[i].go==GO_DOWN)  {if((++enemy[i].y)>>4>=enemy[i].bottom) {enemy[i].go=GO_UP;}} else 
        if(enemy[i].go==GO_LEFT)  {if((--enemy[i].x)>>4<=enemy[i].left)   {enemy[i].go=GO_RIGHT;}} else
        if(enemy[i].go==GO_UP)    {if((--enemy[i].y)>>4<=enemy[i].top)    {enemy[i].go=GO_DOWN;}}        
    }
}

void enemy_update(void){
    static const u8 *msprite;
    static u8 tick=0;
    static u8 pixels=0;
    u8 i=0;
    for(i=0;i<bogie_count; i++)
    {
        enemy_move(i);
        oam_meta_spr_pal(enemy[i].x, enemy[i].y, ENEMY_PALETTE, enemy[i].msprite);

        msprite = enemy[i].directions[enemy[i].go];
        if(msprite) enemy[i].msprite = msprite;
    }
}

enemy_status_t enemy_event(u8 ship_x, u8 ship_y)
{
    u8 i=0;
    for(i=0;i<bogie_count; i++)
    {
        if(
            enemy[i].x - 8 <= ship_x && ship_x <= enemy[i].x + 8 &&
            enemy[i].y - 8 <= ship_y && ship_y <= enemy[i].y + 8
        )
        {
            return ENEMY_LOSS;
        }
    }
    return NOT_DEAD_YET;
}

