/*
 * buffer.h
 *
 *  Created on: May 12, 2021
 *      Author: Protuberanec
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stm32f0xx.h>

struct BUFFER {

};

void Buffer_init(struct BUFFER* buffer, uint8_t* array, int size);
void Buffer_flush(struct BUFFER* buffer);

uint8_t Buffer_AddToEnd(struct BUFFER* buffer, uint8_t data);	//if 1 buffer full, 0 - OK
uint8_t Buffer_GetFromFront(struct BUFFER* buffer, uint8_t* data); //0 - OK, 1 - empty



#endif /* BUFFER_H_ */
