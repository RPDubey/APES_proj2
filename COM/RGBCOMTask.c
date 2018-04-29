/*
 * RGBTask.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */
#include <COM.h>
#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <stdio.h>
#include <string.h>

#include "utils/uartstdio.h"
#include "driverlib/i2c.h"
#include "common.h"

TaskHandle_t RGBHandle;
extern SemaphoreHandle_t xSemaphore ;

void RGBTask(void* pvParameters)
{

    Socket_t *xClientSocket = (Socket_t*)pvParameters;

      /***buffer for tx/rx data*****/
      msg_struct *tx_buf= (msg_struct*)pvPortMalloc(PACKET_SIZE);
      configASSERT(tx_buf != NULL);

      tx_buf->dev_ID = DEV_ID;
    //    tx_buf->task_ID = 2;//DEVICE_ID;
      tx_buf->msg_type = 3 ;
      strcpy(tx_buf->message, "Hello 1234567");
    //
    //    msg_struct *rx_buf= (msg_struct*)pvPortMalloc(PACKET_SIZE);
    //    configASSERT(rx_buf != NULL);
    //    BaseType_t xBytesSent;
    //
    //  vTaskDelay(pdMS_TO_TICKS(500));
     BaseType_t xBytesSent = FreeRTOS_send(*xClientSocket, /* The socket being sent to. */
                                 (void*) (tx_buf),/* The data being sent. */
                                 PACKET_SIZE,/* The remaining length of data to send. */
                                 0); /* ulFlags. */
      if (xBytesSent >= 0)  {UARTprintf("\nBytes Sent:%d", xBytesSent);}
      else UARTprintf("send failed\n");


    for (;;)
    {

        UARTprintf("aloha\n");

vTaskDelay(pdMS_TO_TICKS(2000));
    }

    vTaskDelete(NULL);
}

