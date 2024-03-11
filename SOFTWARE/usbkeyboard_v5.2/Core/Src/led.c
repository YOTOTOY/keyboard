#include "stm32f1xx.h"
#include "led.h"

uint8_t  USB_Recive_Buffer[2];
uint8_t  USB_Received_Count;

void led(void)
{
  if(USB_Recive_Buffer[1] & 0x02)
  {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, HIGH);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, LOW);
  }                                           //大小写指示灯
}
