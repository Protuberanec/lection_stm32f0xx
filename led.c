/*
 * led.c
 *
 *  Created on: 15 мая 2021 г.
 *      Author: Tr
 */

#include "led.h"

void InitLed() {
	InitGPIOC();
}

void TurnOnLed(int num_led) {
	SET_PC(num_led, 1);
}
void TurnOffLed(int num_led) {
	SET_PC(num_led, 0);
}


