#include "My_Lib_Servo.h"

Servo_t sv1;
extern TIM_HandleTypeDef htim3;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void pwm_set_duty(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t duty)
{
  // duty: 0-100
  uint16_t ccr = duty * ((htim->Instance->ARR+1)/100);
  __HAL_TIM_SET_COMPARE(htim, Channel, ccr);
  // HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void servo_write(Servo_t *sv, uint8_t angle)
{
  // angle: 0-180
  uint16_t ccr = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  __HAL_TIM_SET_COMPARE(sv->htim, sv->Channel, ccr);
  // HAL_TIM_PWM_Start(sv->htim, sv->Channel);
}

void servo_init(Servo_t *sv, TIM_HandleTypeDef *htim, uint32_t Channel)
{
    htim->Instance->PSC = 95;
    htim->Instance->ARR = 19999;
    sv->htim = htim;
    sv->Channel = Channel;
}

/*
  Normally test LED with f=[500:1000]Hz
  Using led connect with output of Timer PWM pin to check.
*/
void test_pwm_Led(void)
{
  uint8_t i;
  for (i= 1; i< 99;i++)
  {
    pwm_set_duty(&htim3, TIM_CHANNEL_1, i);
    HAL_Delay(5);
  }
  for (i= 99; i>=1;i--)
  {
    pwm_set_duty(&htim3, TIM_CHANNEL_1, i);
    HAL_Delay(5);
  }
}

void test_pwm_Servo(void)
{
  uint8_t i;
  for (i= 1; i< 180;i++)
  {
    // servo_write(&htim9, TIM_CHANNEL_2, i);
    servo_write(&sv1, i);
    HAL_Delay(5);
  }
  for (i= 180; i>=1;i--)
  {
    // servo_write(&htim9, TIM_CHANNEL_2, i);
    servo_write(&sv1, i);
    HAL_Delay(5);
  }
}

