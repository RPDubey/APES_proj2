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

extern SemaphoreHandle_t xSemaphore;
extern SemaphoreHandle_t RGB_sem;
extern QueueHandle_t COM_que;
QueueHandle_t RGB_Que;

void RGBTask(void* pvParameters)
{

    BaseType_t ret;
    RGB_Que = xQueueCreate(MAX_QUE_SIZE,PACKET_SIZE);
    configASSERT(RGB_Que != NULL);


#ifdef USE_SOCKET
    //signal sensor task to proceed
//       BaseType_t ret;
//       UARTprintf("\nRGB sem given");
//       ret = xSemaphoreGive(RGB_sem );
//       configASSERT(ret == pdTRUE)
    tx_buf->msg_type = COM_REQ;
    strcpy(tx_buf->message, "RGB SENSOR COM ");
    Socket_t *xClientSocket = (Socket_t*)pvParameters;
    BaseType_t xBytesSent = FreeRTOS_send(*xClientSocket, /* The socket being sent to. */
            (void*) (tx_buf),/* The data being sent. */
            PACKET_SIZE,/* The remaining length of data to send. */
            0); /* ulFlags. */
    if (xBytesSent >= 0)
    {   UARTprintf("\nBytes Sent:%d", xBytesSent);}
    else UARTprintf("send failed\n");
#endif

#ifdef USE_UART
    UARTprintf("\nSending RGB Data through UART");
#endif

    /***buffer for tx/rx data*****/
       msg_struct *tx_buf = (msg_struct*)pvPortMalloc(PACKET_SIZE);
       configASSERT(tx_buf != NULL);


//    signal sensor task to proceed and start collecting data
        ret = xSemaphoreGive(RGB_sem );
        configASSERT(ret == pdTRUE)

    for (;;)
    {
        //collect data from sensor
        ret = xQueueReceive(RGB_Que, tx_buf, portMAX_DELAY);//block on receive
        configASSERT(ret == pdPASS);

#ifdef USE_UART
        //send it on que to uart task
        ret = xQueueSendToFront(COM_que, (void * )tx_buf, pdMS_TO_TICKS(3000));
        if (ret != pdPASS)
            UARTprintf("Que Full");
#endif

#ifdef USE_SOCKET
#endif
    }

    vTaskDelete(NULL);
}

