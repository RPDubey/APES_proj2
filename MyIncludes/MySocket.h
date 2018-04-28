/*
 * MySocket.h
 *
 *  Created on: Apr 21, 2018
 *      Author: Ravi
 */

#ifndef MYINCLUDES_MYSOCKET_H_
#define MYINCLUDES_MYSOCKET_H_

#define PORT 100

void SocketServerTask(void* pvParameters);
void COMSocketClientTask(void* pvParameters);
void SocketTask(void* pvParameters);

#endif /* MYINCLUDES_MYSOCKET_H_ */
