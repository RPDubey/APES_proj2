/*******************************************************************************
   @Filename:includes.h
   @Brief:contains common definitions
   @Author:Ravi Dubey
   @Date:3/14/2018
 ******************************************************************************/
#ifndef includes_H
#define includes_H

#include<stdint.h>
/*******************Compile Time Switches*************************************/
//#define USE_SOCKET
 #define USE_UART



/*****************************************************************************/
#define COM_TASK_PORT 1000

#define COM      (uint32_t)0
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
#define MAX_TASKS 2
#define PRINT_ERR(err)                                                         \
  {                                                                            \
    printf("%s Error: %s\n", err, strerror(errno));                            \
    return -1;                                                                 \
  }


//#define PORT 8080
#define DEFAULT_FILE_NAME ("logfile.txt")

#define SLEEP(t)                                                               \
  struct timespec current, remaining;                                          \
  current.tv_nsec = 0;                                                         \
  current.tv_sec = t;                                                          \
  do {                                                                         \
    ret = nanosleep(&current, &remaining);                                     \
    if (ret == -1) {                                                           \
      current.tv_sec = remaining.tv_sec;                                       \
      current.tv_nsec = remaining.tv_nsec;                                     \
    }                                                                          \
  } while (ret != 0);
char* filename;

#define DEFAULT_THREAD_ATTR ((void *)0)
#define LED_ON (system("echo 1 > /sys/class/leds/beaglebone:green:usr0/brightness"))
#define LED_OFF (system("echo 0 > /sys/class/leds/beaglebone:green:usr0/brightness"))
#define LED_CONTROL(status) {if(status == 1) LED_ON; \
                             if(status == 0) LED_OFF; }

#define READY_LED 		(system("echo none >/sys/class/leds/beaglebone:green:usr1/trigger"))
#define INTR_LED_ON 		(system("echo 1 > /sys/class/leds/beaglebone:green:usr1/brightness"))

#define INTR_LED_OFF		(system("echo 0 > /sys/class/leds/beaglebone:green:usr1/brightness"))

#endif
