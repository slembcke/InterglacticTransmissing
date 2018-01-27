#ifndef SNAKE_H__
#define SNAKE_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"

/*
Send us events from main like this

    if(JOY_UP(joy)){
        snake_event(0,0,EVENT_UP);
    }
    if(JOY_DOWN(joy)){
        snake_event(0,0,EVENT_DW);
    }
    if(JOY_LEFT(joy)){
        snake_event(0,0,EVENT_LF);
    }
    if(JOY_RIGHT(joy)){
        snake_event(0,0,EVENT_RT);
    }
*/


#define EVENT_UP 0
#define EVENT_DW 1
#define EVENT_LF 2
#define EVENT_RT 3

extern void snake_init(void);
extern void snake_task(void);
extern void snake_draw_task(void);
extern void snake_event(unsigned char x, unsigned char y, unsigned char dir);

#endif //SNAKE_H__
