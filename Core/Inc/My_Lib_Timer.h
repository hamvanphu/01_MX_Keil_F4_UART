#ifndef _MY_LIB_TIMER_H
#define _MY_LIB_TIMER_H
#include "stm32f4xx_hal.h"

typedef enum
{
    TIMER_10ms,
    TIMER_25ms,
    TIMER_100ms,
    TIMER_200ms,
    TIMER_1000ms,
    NUM_OF_TIMER,
}USER_TIMER_ID;

#define TIME_COUNT_10MS               10
#define TIME_COUNT_25MS               25
#define TIME_COUNT_100MS              100
#define TIME_COUNT_200MS              200
#define TIME_COUNT_1000MS             1000

extern uint32_t volatile userTimerTick_1ms;

uint32_t User_GetTimerTick(void);
static void User_UpdateGlobalTimer(void);
void User_SetTimer(USER_TIMER_ID id, uint32_t ms);
uint32_t User_GetTimer(USER_TIMER_ID id);
void AppTimerHandler(void); // Call in WHILE TRUE

#endif //_MY_LIB_TIMER_H
