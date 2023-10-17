#include "main.h"
#include "HC_SR04.h"

#define HCSR04_LIB_ECHO_PIN             SR04_ECHO_Pin
#define HCSR04_LIB_ECHO_PORT            SR04_ECHO_GPIO_Port
#define HCSR04_LIB_ECHO_EXTI_IRQ        SR04_ECHO_EXTI_IRQn
#define HCSR04_LIB_TRIGER_PIN           SR04_TRIGER_Pin
#define HCSR04_LIB_TRIGER_PORT          SR04_TRIGER_GPIO_Port

HCSR04_State_t hc_state = HCSR04_IDLE_STATE;
bool is_HC_ok = HCSR04_SENSOR_OK; // OK or NG
float hc_Distance = 0;
long time_debug = 0;
long time_debug1 = 0;

extern TIM_HandleTypeDef htim4;

void HCSR04_Trigger(void)
{
  static uint32_t t_old;
  if(HAL_GetTick() - t_old >= 500) // 500ms
  {
    if(hc_state == HCSR04_IDLE_STATE)
    {
      HAL_GPIO_WritePin(HCSR04_LIB_TRIGER_PORT, HCSR04_LIB_TRIGER_PIN, GPIO_PIN_SET);
      hc_state = HCSR04_GEN_PULSE_STATE;
    }
    t_old = HAL_GetTick();
  }
}
void HCSR04_Callback(void)
{
  if(HAL_GPIO_ReadPin(HCSR04_LIB_ECHO_PORT, HCSR04_LIB_ECHO_PIN) == GPIO_PIN_SET) // Raising trigger
  {
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    HAL_TIM_Base_Start(&htim4);
  }
  else // Falling trigger
  {
    hc_Distance = __HAL_TIM_GET_COUNTER(&htim4)*0.017;
    HAL_TIM_Base_Stop(&htim4);
    hc_state = HCSR04_DONE_STATE;
  }
}
void HCSR04_Handler(void)
{
  switch (hc_state)
  {
    case HCSR04_IDLE_STATE:
      printf("hc_state HCSR04_IDLE_STATE\r\n");
      break;
    case HCSR04_GEN_PULSE_STATE:
      printf("hc_state HCSR04_GEN_PULSE_STATE\r\n");
      if(HAL_GetTick() - time_debug >= 1) // 1ms
      {
        time_debug = HAL_GetTick();
        HAL_GPIO_WritePin(HCSR04_LIB_TRIGER_PORT, HCSR04_LIB_TRIGER_PIN, GPIO_PIN_RESET);
        hc_state = HCSR04_START_STATE;
      }
      break;
    case HCSR04_START_STATE:
      printf("hc_state HCSR04_START_STATE\r\n");
      if(HAL_GetTick() - time_debug1 >= 2000) // 2s
      {
        time_debug1 = HAL_GetTick();
        // Sensor has problem!
        printf("Sensor has problem!\r\n");
        is_HC_ok = HCSR04_SENSOR_NG;
        hc_state = HCSR04_IDLE_STATE;
      }
      break;
    case HCSR04_DONE_STATE:
      printf("hc_state HCSR04_DONE_STATE\r\n");
      is_HC_ok = HCSR04_SENSOR_OK;
      hc_state = HCSR04_IDLE_STATE;
      // For example: Print result to LCD or LED control
      // if(hc_Distance > 10)
      // {
      //   BSP_LED_On(LED3);
      // }
      // else
      // {
      //   BSP_LED_Off(LED3);
      // }
      break;
    default:
      break;
  }
}