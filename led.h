/*
 * led.h
 *
 *  Created on: 15 мая 2021 г.
 *      Author: Tr
 */

#ifndef LED_H_
#define LED_H_

#include <stm32f0xx.h>
#include "my_gpio.h"

#define LED1	8
#define LED9	9

void InitLed();
void TurnOnLed(int num_led);
void TurnOffLed(int num_led);


#endif /* LED_H_ */
