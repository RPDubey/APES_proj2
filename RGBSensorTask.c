/*
 * RGBSensorTask.c
 *
 *  Created on: Apr 29, 2018
 *      Author: Ravi
 */

#include <stdio.h>
#include <string.h>
#include <SensorTasks.h>

#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

#include "MyI2C.h"


#include "utils/uartstdio.h"
#include "driverlib/i2c.h"
#include "common.h"

extern SemaphoreHandle_t RGB_sem ;

void RGBSensorTask(void* pvParameters)
{

//initialize sensor

//wait for com layer to be ready

//send to com layer on loop
#ifndef TEST
    uint8_t read_val;
    RGB_SENSOR_REG_t reg, reg1, reg2;

    //reset
    reg = DEVICE_ID;
    RGB_SENSOR_WRITE(0x46, reg);
    SysCtlDelay(1000);

    //check for device id
    UARTprintf("\nConfig1:%x",RGB_SENSOR_READ(reg));

    //configuration
    reg = CONFIG_1;
    RGB_SENSOR_WRITE(CFG1_MODE_RGB | CFG1_10KLUX | CFG1_ADC_SYNC_NORMAL, reg); //| CFG1_16BIT
    UARTprintf("\nConfig1:%x", RGB_SENSOR_READ(reg));

    reg = CONFIG_2;
    RGB_SENSOR_WRITE(CFG2_IR_OFFSET_ON | CFG2_IR_ADJUST_HIGH, reg);
    UARTprintf("\nConfig2:%x", RGB_SENSOR_READ(reg));

    reg = CONFIG_3;
    RGB_SENSOR_WRITE(CFG3_R_INT | CFG3_INT_PRST4 | CFG3_RGB_CONV_TO_INT_DISABLE,
                     reg);
    UARTprintf("\nConfig3:%x", RGB_SENSOR_READ(reg));

//set interrupt threshold
    reg1 = THRESHOLD_LH;
    reg2 = THRESHOLD_LL;
    RGB_SENSOR_WRITE(0x00, reg1);
    RGB_SENSOR_WRITE(0x00, reg2);
    UARTprintf("\nThreshold LOW:%x,%x", RGB_SENSOR_READ(reg1),
               RGB_SENSOR_READ(reg2));

    reg1 = THRESHOLD_HH;
    reg2 = THRESHOLD_HL;
    RGB_SENSOR_WRITE(0x00, reg1);
    RGB_SENSOR_WRITE(0xFF, reg2);
    UARTprintf("\nThreshold HIGH:%x,%x", RGB_SENSOR_READ(reg1),
               RGB_SENSOR_READ(reg2));

#else
    UARTprintf("\nRGB initialization done");
#endif

    //xSemaphoreTake(RGB_sem, portMAX_DELAY  );
    UARTprintf("\nRGB sem taken");


    for (;;)
    {
#ifndef TEST
        read_val = 69;

        reg = STATUS;
        UARTprintf("  STATUS:%x", RGB_SENSOR_READ(reg));

        reg1 = RED_H;
        reg2 = RED_L;
        UARTprintf("  RED:%x,%x", RGB_SENSOR_READ(reg1), RGB_SENSOR_READ(reg2));

        reg1 = GREEN_H;
        reg2 = GREEN_L;
        UARTprintf("  GREEN:%x,%x", RGB_SENSOR_READ(reg1),
                   RGB_SENSOR_READ(reg2));

        reg1 = BLUE_H;
        reg2 = BLUE_L;
        UARTprintf("  BLUE:%x,%x", RGB_SENSOR_READ(reg1),
                   RGB_SENSOR_READ(reg2));
#else
        UARTprintf("\nRGB");
#endif


        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    vTaskDelete(NULL);
}
