#ifndef __HC_SR04_H
#define __HC_SR04_H

#define OK  1
#define NG  0
#define HCSR04_SENSOR_OK    OK
#define HCSR04_SENSOR_NG    NG

typedef enum
{
  HCSR04_IDLE_STATE,
  HCSR04_GEN_PULSE_STATE,
  HCSR04_START_STATE,
  HCSR04_DONE_STATE,
}HCSR04_State_t;

void HCSR04_Trigger(void);
void HCSR04_Callback(void);
void HCSR04_Handler(void);

#endif /*__HC_SR04_H*/
