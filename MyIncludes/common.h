/*
 * common.h
 *
 *  Created on: Apr 17, 2018
 *      Author: Ravi
 */

#ifndef MYINCLUDES_COMMON_H_
#define MYINCLUDES_COMMON_H_

/*************** Compile time switches ***********************/
#define USE_SOCKET
//#define USE_UART

/************************************************************/
#define STACK_DEPTH           1024 //1024 words


typedef enum {
  com = (int)0,
  init = (int)1,
  HB = (int)2,
  error = (int)3,
  log = (int)4
} msg_type_t;

typedef struct {
  uint8_t dev_ID;
  uint32_t msg_type;
  char message[20];
} msg_struct;

#define PACKET_SIZE sizeof(msg_struct)

#endif /* MYINCLUDES_COMMON_H_ */
