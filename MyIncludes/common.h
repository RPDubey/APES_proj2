/*
 * common.h
 *
 *  Created on: Apr 17, 2018
 *      Author: Ravi
 */

#ifndef MYINCLUDES_COMMON_H_
#define MYINCLUDES_COMMON_H_

#include<stdint.h>
/*************** Compile time switches ***********************/
#define USE_SOCKET
//#define USE_UART

//#define TEST
/************************************************************/
#define MAX_TASKS 2
#define DEV_ID 1
#define RGB_TASK_ID 1
#define ZX_TASK_ID  2

#define MAX_QUE_SIZE 10

#define SERVER_IP_ADDRESS   FreeRTOS_inet_addr_quick(192,168,1,7)
#define DEFAULT_SERVER_IP_ADDRESS   FreeRTOS_inet_addr_quick(192,168,1,35)


#define STACK_DEPTH           1024 //1024 words



#define COM_REQ  (uint32_t)0
#define INIT     (uint32_t)1
#define HB       (uint32_t)2
#define ERROR    (uint32_t)3
#define LOG      (uint32_t)4
#define COM_ACK  (uint32_t)5



typedef struct {
  uint8_t dev_ID;
  uint8_t task_ID;
  uint8_t msg_type;
  uint8_t msg_val;
  char message[20];
} msg_struct;

#define PACKET_SIZE sizeof(msg_struct)

#endif /* MYINCLUDES_COMMON_H_ */
