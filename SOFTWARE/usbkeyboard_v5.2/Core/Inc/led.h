#ifndef __LED_H__
#define __LED_H__

#include "stm32f1xx_hal.h"

#define HIGH GPIO_PIN_SET
#define LOW  GPIO_PIN_RESET

extern uint8_t  USB_Recive_Buffer[2];
extern uint8_t  USB_Received_Count;

void led(void);

#endif
