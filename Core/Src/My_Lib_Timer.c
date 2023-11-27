/*
Please use userTimerTick_1ms and place it into Timer 1ms interupt service.
*/

#include "My_Lib_Timer.h"

static uint32_t userGlobalTimer[NUM_OF_TIMER] = {0};
uint32_t volatile userTimerTick_1ms;

uint32_t User_GetTimerTick(void)
{
    uint32_t timer;
    do
    {
        timer = userTimerTick_1ms;
    } while (timer != userTimerTick_1ms);
    return timer;
}

static void User_UpdateGlobalTimer(void)
{
    static uint32_t localtimer = 0;
    uint32_t count, tmp;
    uint8_t i;

    tmp = User_GetTimerTick();
    count = tmp - localtimer;
    localtimer = tmp;

    for(i=0; i<NUM_OF_TIMER; i++)
    {
        if(count > userGlobalTimer[i])
        {
            userGlobalTimer[i] = 0;
        }
        else
        {
            userGlobalTimer[i] = count;
        }
    }
}

void User_SetTimer(USER_TIMER_ID id, uint32_t ms)
{
    User_UpdateGlobalTimer();
    userGlobalTimer[id] = ms;
}

uint32_t User_GetTimer(USER_TIMER_ID id)
{
    User_UpdateGlobalTimer();
    return userGlobalTimer[id];
}

void AppTimerHandler(void) // Call in WHILE TRUE
{
    if(User_GetTimer(TIMER_10ms)) // tasks which need to run every 10ms
    {
        User_SetTimer(TIMER_10ms, TIME_COUNT_10MS);
        //TODO
    }

    if(User_GetTimer(TIMER_25ms)) // tasks which need to run every 25ms
    {
        User_SetTimer(TIMER_10ms, TIME_COUNT_25MS);
        //TODO
    }

    if(User_GetTimer(TIMER_100ms)) // tasks which need to run every 100ms
    {
        User_SetTimer(TIMER_10ms, TIME_COUNT_100MS);
        //TODO
    }

    if(User_GetTimer(TIMER_200ms)) // tasks which need to run every 200ms
    {
        User_SetTimer(TIMER_10ms, TIME_COUNT_200MS);
        //TODO
    }

    if(User_GetTimer(TIMER_1000ms)) // tasks which need to run every 1000ms
    {
        User_SetTimer(TIMER_10ms, TIME_COUNT_1000MS);
        //TODO
    }

}