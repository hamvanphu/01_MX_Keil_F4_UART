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
extern volatile uint32_t delayCounter;

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
#define DELAY_TYPE 2
#if (DELAY_TYPE == 1)
void delay_us(uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim4, 0);
  __HAL_TIM_SET_AUTORELOAD(&htim4, delay - 1);
  __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
  HAL_TIM_Base_Start_IT(&htim4);
  while(!__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE));
  HAL_TIM_Base_Stop_IT(&htim4);
}
#elif (DELAY_TYPE == 2)
void delay_us(uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim4, 0);

  // Set auto-reload value to achieve desired delay
  __HAL_TIM_SET_AUTORELOAD(&htim4, delay - 1);

  // Clear the update flag
  __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);

  // Start TIM4
  HAL_TIM_Base_Start(&htim4);

  // Wait until TIM4 reaches the auto-reload value
  while (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) == RESET);

  __HAL_TIM_SET_COUNTER(&htim4, 0);

  // Stop TIM4
  HAL_TIM_Base_Stop(&htim4);
}
#elif ((DELAY_TYPE == 3))
void delay_us(uint16_t delay)
{
  delayCounter = delay;
  HAL_TIM_Base_Start_IT(&htim4);
  while (delayCounter > 0)
  {
    // Wait for the delay to complete
  }
  HAL_TIM_Base_Stop_IT(&htim4);
}
#else
#endif

float HCSR04_ReadDistance() {
    // Send a 10us pulse on TRIG pin
    HAL_GPIO_WritePin(HCSR04_LIB_TRIGER_PORT, HCSR04_LIB_TRIGER_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(HCSR04_LIB_TRIGER_PORT, HCSR04_LIB_TRIGER_PIN, GPIO_PIN_RESET);

    // Wait for the ECHO pin to go high
    uint32_t timeout = 1000000;  // Timeout in us
    while (HAL_GPIO_ReadPin(HCSR04_LIB_ECHO_PORT, HCSR04_LIB_ECHO_PIN) == GPIO_PIN_RESET) {
        if (timeout == 0) {
            return -1;  // Timeout occurred
        }
        timeout--;
    }

    // Start the timer
    HAL_TIM_Base_Start(&htim4);

    // Wait for the ECHO pin to go low
    timeout = 1000000;  // Timeout in us
    while (HAL_GPIO_ReadPin(HCSR04_LIB_ECHO_PORT, HCSR04_LIB_ECHO_PIN) == GPIO_PIN_SET) {
        if (timeout == 0) {
            return -1;  // Timeout occurred
        }
        timeout--;
    }

    // Stop the timer and calculate the pulse duration
    HAL_TIM_Base_Stop(&htim4);
    uint16_t pulse_duration = __HAL_TIM_GET_COUNTER(&htim4);

    // Calculate the distance based on the pulse duration (in cm)
    float distance = pulse_duration / 58.0; // instead of *0.017

    return distance;
}