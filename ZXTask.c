/*
 * ZXTask.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */


#include "MyTasks.h"
#include "utils/uartstdio.h"
#include "common.h"


#define ZX_SENSOR_ADDR           0x10

#define ZX_SENSOR_WRITE(DataTx,reg) I2C0_Master_Write(ZX_SENSOR_ADDR,DataTx,reg)
#define ZX_SENSOR_READ(reg)     I2C0_Master_Read(ZX_SENSOR_ADDR,reg)


typedef enum{
    ZX_STATUS =    0x00,
    DRE    =    0x01,
    DRCFG  =    0x02,
    GESTURE=    0x04,
    GSPEED =    0x05,
    DCM    =    0x06,
    XPOS   =    0x08,
    ZPOS   =    0x0a,
    LRNG   =    0x0c,
    RRNG   =    0x0e,
    REGVER =    0xfe,
    MODEL  =    0xff
}ZX_SENSOR_REG_t;

#define POSITION_DATA_AVAILABLE 0x01
#define SWIPE_GESTURE_AVAILABLE 0x04
#define HOVER_GESTURE_AVAILABLE 0x08
#define HVRMV_GESTURE_AVAILABLE 0x10

void ZXTask(void* pvParameters)
{
    LED_PF0_SETUP
    uint32_t led_state = GPIO_PIN_OFF;
    uint8_t read_val,status;
    ZX_SENSOR_REG_t reg;

    reg = MODEL;
    read_val =  ZX_SENSOR_READ(reg);
    UARTprintf("\nSensor Model:%x",read_val);

    reg = REGVER;
    read_val =  ZX_SENSOR_READ(reg);
    UARTprintf("\nRegister Version:%x",read_val);






    reg = DRCFG;
    read_val =  ZX_SENSOR_READ(reg);
    ZX_SENSOR_WRITE( (uint8_t)(read_val & 0x7F),reg );
    read_val =  ZX_SENSOR_READ(reg);
    UARTprintf("\nDRCFG:%x",read_val);

//    uint32_t NotificationVal = 0x00;
//    BaseType_t ret;

    for (;;)
    {



  xSemaphoreTake(xSemaphore, portMAX_DELAY  );

        led_state = !led_state;
        LED_PFO_ON_OFF(led_state);


    //   UARTprintf("\n");

       reg = ZX_STATUS;
       status =  ZX_SENSOR_READ(reg);
//       UARTprintf("Status:%x",status);

       if(status & (uint8_t)POSITION_DATA_AVAILABLE)
       {
       reg = ZPOS;       read_val = 69;
       read_val =  ZX_SENSOR_READ(reg);
       UARTprintf("\n");

       UARTprintf("  ZPOS:%d",read_val);

//       reg = XPOS;       read_val = 69;
//       read_val =  ZX_SENSOR_READ(reg);
//       UARTprintf("  XPOS:%d",read_val);
       }


//       if(status & (uint8_t)SWIPE_GESTURE_AVAILABLE ){
//           reg = GESTURE;
//           read_val =  ZX_SENSOR_READ(reg);
//           UARTprintf("  GESTURE:%x",read_val);

//       }


    }

    vTaskDelete(NULL);
}
