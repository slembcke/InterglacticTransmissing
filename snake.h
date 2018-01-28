#ifndef SNAKE_H__
#define SNAKE_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <joystick.h>
#include <nes.h>
#include "neslib/neslib.h"
#include "main.h"

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

typedef enum snake_status {SNAKE_LOSS, SNAKE_WIN, IN_PROGRESS} snake_status_t;

extern void snake_init(void);
extern void snake_task(void);
extern void snake_draw_task(void);
extern void snake_draw_post(void);
extern void snake_event(u8 x, u8 y, s16 vx, s16 vy);
extern snake_status_t snake_success(void);


#endif //SNAKE_H__
