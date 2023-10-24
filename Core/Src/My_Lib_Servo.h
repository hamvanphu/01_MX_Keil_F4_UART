#ifndef __MY_LIB_SERVO_H
#define __MY_LIB_SERVO_H
#include "main.h"

//Refered Arduino code and already calib
#define MIN_PULSE_WIDTH   544 // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH   2100 // the longest pulse sent to a servo

typedef struct
{
  TIM_HandleTypeDef *htim;
  uint32_t Channel;
  uint8_t angle;
}Servo_t;

extern Servo_t sv1;

long map(long x, long in_min, long in_max, long out_min, long out_max);
void servo_write(Servo_t *sv, uint8_t angle);
void servo_init(Servo_t *sv, TIM_HandleTypeDef *htim, uint32_t Channel);
void pwm_set_duty(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t duty);
void test_pwm_Led(void);
void test_pwm_Servo(void);

#endif
