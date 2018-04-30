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


const uint8_t   DEVICE_ID = 0x00;
const uint8_t    CONFIG_1 =  0x01;
const uint8_t   CONFIG_2 = 0x02;
const uint8_t   CONFIG_3 = 0x03;
const uint8_t   THRESHOLD_LL = 0x04;
const uint8_t   THRESHOLD_LH = 0x05;
const uint8_t   THRESHOLD_HL = 0x06;
const uint8_t   THRESHOLD_HH = 0x07;
const uint8_t   STATUS = 0x08 ;
const uint8_t   GREEN_L = 0x09 ;
const uint8_t   GREEN_H = 0x0A;
const uint8_t   RED_L = 0x0B;
const uint8_t   RED_H = 0x0C;
const uint8_t   BLUE_L = 0x0D;
const uint8_t   BLUE_H =0x0E;

extern SemaphoreHandle_t RGB_sem ;
extern SemaphoreHandle_t xSemaphore;

extern QueueHandle_t RGB_Que;


void RGBSensorTask(void* pvParameters)
{
    BaseType_t ret;
    xSemaphoreTake(RGB_sem, portMAX_DELAY  );

    /***buffer for tx/rx data*****/
    msg_struct *tx_buf = (msg_struct*)pvPortMalloc(PACKET_SIZE);
    configASSERT(tx_buf != NULL);
    tx_buf->dev_ID = DEV_ID;

    #ifndef TEST
    uint8_t read_val;
uint8_t reg1,reg2,reg3;

    RGB_SENSOR_WRITE(0x46, DEVICE_ID);
    SysCtlDelay(1000);

    //configuration
    RGB_SENSOR_WRITE(CFG1_MODE_RGB | CFG1_10KLUX | CFG1_ADC_SYNC_NORMAL, CONFIG_1); //| CFG1_16BIT
    UARTprintf("\nConfig1:%x", RGB_SENSOR_READ(CONFIG_1));

    RGB_SENSOR_WRITE(CFG2_IR_OFFSET_ON | CFG2_IR_ADJUST_HIGH, CONFIG_2);
    UARTprintf("\nConfig2:%x", RGB_SENSOR_READ(CONFIG_2));

    RGB_SENSOR_WRITE(CFG3_R_INT | CFG3_INT_PRST4 | CFG3_RGB_CONV_TO_INT_DISABLE,
                     CONFIG_3);
    UARTprintf("\nConfig3:%x", RGB_SENSOR_READ(CONFIG_3));

//set interrupt threshold
    reg1 = THRESHOLD_LH;
    reg2 = THRESHOLD_LL;
    RGB_SENSOR_WRITE(0x00, THRESHOLD_LH);
    RGB_SENSOR_WRITE(0x00, THRESHOLD_LL);
    UARTprintf("\nThreshold LOW:%x,%x", RGB_SENSOR_READ(reg1),
               RGB_SENSOR_READ(reg2));

    reg1 = THRESHOLD_HH;
    reg2 = THRESHOLD_HL;
    RGB_SENSOR_WRITE(0x00, THRESHOLD_HH);
    RGB_SENSOR_WRITE(0xA0, THRESHOLD_HL);
    UARTprintf("\nThreshold HIGH:%x,%x", RGB_SENSOR_READ(reg1),
               RGB_SENSOR_READ(reg2));

#endif

    strcpy(tx_buf->message,"RGB INIT Done");
    tx_buf->msg_type = INIT;
    tx_buf->msg_val = 1;
    tx_buf->task_ID = RGB_TASK_ID;

    ret =  xQueueSendToFront( RGB_Que,(void *)tx_buf,pdMS_TO_TICKS(3000));
    if(ret!= pdPASS)  UARTprintf("Que Full");

    for (;;)
    {
#ifndef TEST
        read_val = 69;

         RGB_SENSOR_READ(STATUS);//clear interrupts
         tx_buf->msg_type = LOG;

        reg1 = RGB_SENSOR_READ(GREEN_H);
        reg2 = RGB_SENSOR_READ(GREEN_L);
        UARTprintf("  GREEN:%x,%x",reg1 , reg2);
        strcpy(tx_buf->message,"GREEN");
        tx_buf->msg_val = reg2;
        ret =  xQueueSendToBack( RGB_Que,(void *)tx_buf,pdMS_TO_TICKS(3000));
        if(ret!= pdPASS)  UARTprintf("Que Full");

        reg1 = RGB_SENSOR_READ(BLUE_H);
        reg2 = RGB_SENSOR_READ(BLUE_L);
        UARTprintf("  BLUE:%x,%x",reg1 , reg2);
        strcpy(tx_buf->message,"BLUE");
        tx_buf->msg_val = reg2;
        ret =  xQueueSendToBack( RGB_Que,(void *)tx_buf,pdMS_TO_TICKS(3000));
        if(ret!= pdPASS)  UARTprintf("Que Full");


        reg1 = RGB_SENSOR_READ(RED_H);
        reg2 = RGB_SENSOR_READ(RED_L);
        UARTprintf("  RED:%x,%x",reg1 , reg2);
        strcpy(tx_buf->message,"Red");
        tx_buf->msg_val = reg2;
        UARTprintf("\n");
#endif
        ret =  xQueueSendToBack( RGB_Que,(void *)tx_buf,pdMS_TO_TICKS(3000));
        if(ret!= pdPASS)  UARTprintf("Que Full");

        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    vTaskDelete(NULL);
}
