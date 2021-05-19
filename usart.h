/*
 * usart.h
 *
 *  Created on: May 12, 2021
 *      Author: Protuberanec
 */

#ifndef USART_H_
#define USART_H_

#include <stm32f0xx.h>

#define BAUD	115200
#define SIZE_TX_DATA	128
#define DMA_USE

void USART1_init();
void USART1_DmaTXInit();
void USART1_DmaRXInit();

void USART1_SendDataViaDma(uint8_t* data, int size);
void USART1_GetDataViaDma(uint8_t* data, int size);

uint8_t USART1_SendData(uint8_t data);



#endif /* USART_H_ */
