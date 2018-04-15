/*
 * MyTasks.h
 *
 *  Created on: Apr 10, 2018
 *      Author: Ravi
 */

#ifndef MYTASKS_H_
#define MYTASKS_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"

#include "MyI2C.h"

#define LED_PF0_SETUP     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);\
                          while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));\
                          GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

#define LED_PFO_ON_OFF(led_state) {GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, led_state);}

#define GPIO_PIN_ON           1
#define GPIO_PIN_OFF          0

//Global Variables
TaskHandle_t RGBHandle, ZXHandle;

void RGBTask(void* pvParameters);

void ZXTask(void* pvParameters);

#endif /* MYTASKS_H_ */
