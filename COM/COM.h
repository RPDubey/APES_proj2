/*
 * MySocket.h
 *
 *  Created on: Apr 21, 2018
 *      Author: Ravi
 */

#ifndef MYINCLUDES_COM_H_
#define MYINCLUDES_COM_H_

#define PORT 100

void SocketServerTask(void* pvParameters);

void COMSocketClientTask(void* pvParameters);
void COMUARTClientTask(void* pvParameters);

void SocketTask(void* pvParameters);


void RGBTask(void* pvParameters);

void ZXTask(void* pvParameters);

void HBTask(void* pvParameters);


#endif /* MYINCLUDES_COM_H_ */
