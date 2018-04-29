/*
 * ZXTask.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */


#include "MyTasks.h"
#include "utils/uartstdio.h"
#include "common.h"

#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "MyTasks.h"
#include "MyUart.h"
#include "common.h"

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <stdio.h>
#include <string.h>



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
    Socket_t *xClientSocket = (Socket_t*)pvParameters;

    /***buffer for tx/rx data*****/
    msg_struct *tx_buf= (msg_struct*)pvPortMalloc(PACKET_SIZE);
    configASSERT(tx_buf != NULL);

    tx_buf->dev_ID = DEV_ID;
//    tx_buf->task_ID = 2;//DEVICE_ID;
    tx_buf->msg_type = COM_REQ ;
    strcpy(tx_buf->message, "Hello saranya");
//
//    msg_struct *rx_buf= (msg_struct*)pvPortMalloc(PACKET_SIZE);
//    configASSERT(rx_buf != NULL);
//    BaseType_t xBytesSent;
//
   BaseType_t xBytesSent = FreeRTOS_send(*xClientSocket, /* The socket being sent to. */
                               (void*) (tx_buf),/* The data being sent. */
                               PACKET_SIZE,/* The remaining length of data to send. */
                               0); /* ulFlags. */
    if (xBytesSent >= 0)  {UARTprintf("\nBytes Sent:%d", xBytesSent);}

    while(1){
        UARTprintf("Hello\n");

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
//    LED_PF0_SETUP
//    uint32_t led_state = GPIO_PIN_OFF;
//    uint8_t read_val,status;
//    ZX_SENSOR_REG_t reg;
//
//    reg = MODEL;
//    read_val =  ZX_SENSOR_READ(reg);
//    UARTprintf("\nSensor Model:%x",read_val);
//
//    reg = REGVER;
//    read_val =  ZX_SENSOR_READ(reg);
//    UARTprintf("\nRegister Version:%x",read_val);
//
//
//
//
//
//
//    reg = DRCFG;
//    read_val =  ZX_SENSOR_READ(reg);
//    ZX_SENSOR_WRITE( (uint8_t)(read_val & 0x7F),reg );
//    read_val =  ZX_SENSOR_READ(reg);
//    UARTprintf("\nDRCFG:%x",read_val);
//
////    uint32_t NotificationVal = 0x00;
////    BaseType_t ret;
//
//    for (;;)
//    {
//
//
//
//  xSemaphoreTake(xSemaphore, portMAX_DELAY  );
//
//        led_state = !led_state;
//        LED_PFO_ON_OFF(led_state);
//
//
//    //   UARTprintf("\n");
//
//       reg = ZX_STATUS;
//       status =  ZX_SENSOR_READ(reg);
////       UARTprintf("Status:%x",status);
//
//       if(status & (uint8_t)POSITION_DATA_AVAILABLE)
//       {
//       reg = ZPOS;       read_val = 69;
//       read_val =  ZX_SENSOR_READ(reg);
//       UARTprintf("\n");
//
//       UARTprintf("  ZPOS:%d",read_val);
//
////       reg = XPOS;       read_val = 69;
////       read_val =  ZX_SENSOR_READ(reg);
////       UARTprintf("  XPOS:%d",read_val);
//       }
//
//
////       if(status & (uint8_t)SWIPE_GESTURE_AVAILABLE ){
////           reg = GESTURE;
////           read_val =  ZX_SENSOR_READ(reg);
////           UARTprintf("  GESTURE:%x",read_val);
//
////       }
//
//
//    }

    vTaskDelete(NULL);
}
