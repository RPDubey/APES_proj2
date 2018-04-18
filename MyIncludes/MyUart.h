/*
 * MyUart.h
 *
 *  Created on: Apr 10, 2018
 *      Author: Ravi
 */

#ifndef MYUART_H_
#define MYUART_H_


void UART0Enable();

void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);

#endif /* MYUART_H_ */
