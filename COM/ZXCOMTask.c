/*
 * ZXTask.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */

#include <COM.h>
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


TaskHandle_t ZXHandle;
TaskHandle_t HBHandle;

void ZXTask(void* pvParameters)
{


    //signal sennsor to proceed

    //receive data on que

    //    pass it on to server

    Socket_t *xClientSocket = (Socket_t*) pvParameters;

    /***buffer for tx/rx data*****/
    msg_struct *tx_buf = (msg_struct*) pvPortMalloc(PACKET_SIZE);
    configASSERT(tx_buf != NULL);

    tx_buf->dev_ID = DEV_ID;
//    tx_buf->task_ID = 2;//DEVICE_ID;
    tx_buf->msg_type = COM_REQ;
    strcpy(tx_buf->message, "Hello saranya");
//
//    msg_struct *rx_buf= (msg_struct*)pvPortMalloc(PACKET_SIZE);
//    configASSERT(rx_buf != NULL);
//    BaseType_t xBytesSent;
//
    SysCtlDelay(1000);
    BaseType_t xBytesSent = FreeRTOS_send(*xClientSocket, /* The socket being sent to. */
                                          (void*) (tx_buf),/* The data being sent. */
                                          PACKET_SIZE,/* The remaining length of data to send. */
                                          0); /* ulFlags. */
    if (xBytesSent >= 0)
    {
        UARTprintf("\nBytes Sent:%d", xBytesSent);
    }

    while (1)
    {

        vTaskDelay(pdMS_TO_TICKS(10000));
        UARTprintf("Hello\n");

    }

    vTaskDelete(NULL);
}



void HBTask(void* pvParameters)
{
BaseType_t ret;



    LED_PF0_SETUP
    uint32_t led_state = GPIO_PIN_OFF;

    while (1)
    {

        vTaskSuspend(HBHandle);
        led_state = !led_state;
        LED_PFO_ON_OFF(led_state);

    }

    vTaskDelete(NULL);

}

