/*
 * RGBTask.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */

#include "MyTasks.h"

#define RGB_SENSOR_ADDRESS 0

#define RGB_SENSOR_WRITE(DataTx) {I2C0_Master_Write(RGB_SENSOR_ADDRESS, DataTx);}
#define RGB_SENSOR_READ          I2C0_Master_Read(RGB_SENSOR_ADDRESS);


void RGBTask(void* pvParameters)
{

    LED_PF0_SETUP
    uint32_t led_state = GPIO_PIN_OFF;
    uint8_t read_val;

    for (;;)
    {
        vTaskSuspend(RGBHandle);
        led_state = !led_state;
        LED_PFO_ON_OFF(led_state);

        read_val =  RGB_SENSOR_READ;

    }

    vTaskDelete(NULL);
}

