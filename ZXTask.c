/*
 * ZXTask.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */


#include "MyTasks.h"

#define ZX_SENSOR_ADDR           0x10

#define ZX_SENSOR_WRITE(DataTx) {I2C0_Master_Write(ZX_SENSOR_ADDR, DataTx);}
#define ZX_SENSOR_READ          I2C0_Master_Read(ZX_SENSOR_ADDR);


void ZXTask(void* pvParameters)
{
    LED_PF0_SETUP
    uint32_t led_state = GPIO_PIN_OFF;
    uint8_t read_val;

    for (;;)
    {
        vTaskSuspend(ZXHandle);
        led_state = !led_state;
        LED_PFO_ON_OFF(led_state);

        read_val =  ZX_SENSOR_READ;

    }

    vTaskDelete(NULL);
}
