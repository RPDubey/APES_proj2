/*
 * MyClient.c
 *
 *  Created on: Apr 21, 2018
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
#include <SensorTasks.h>

#include "MyUart.h"
#include "common.h"

//Global Variables
extern TaskHandle_t RGBHandle, ZXHandle;

void COMUARTClientTask(void* pvParameters)
{
    UART6Enable();
    SysCtlDelay(10000);

//create two com  tasks
//    UARTprintf("\nCreating ZX TASK\n");
//    ret = xTaskCreate(ZXTask, "ZX Task", STACK_DEPTH, (void*)(&xClientSocket[task_num]), 1, &ZXHandle);
//    configASSERT(ret == pdPASS);
//
//    UARTprintf("\nCreating RGB TASK\n");
//    ret = xTaskCreate(RGBTask, "RGB Task", STACK_DEPTH, (void*)(&xClientSocket[task_num]), 1, &RGBHandle);
//     configASSERT(ret == pdPASS);

//wait for them to block
    vTaskDelay(pdMS_TO_TICKS(1000));

//block on rcv data from them on que

//send the data to BBG

    msg_struct *tx_buf = (msg_struct*) pvPortMalloc(PACKET_SIZE);
    configASSERT(tx_buf != NULL);

    tx_buf->dev_ID = DEV_ID;
//    tx_buf->task_ID = 2;//DEVICE_ID;
    tx_buf->msg_type = COM_REQ;
    strcpy(tx_buf->message, "Hello saranya abcdef");
//
//    msg_struct *rx_buf= (msg_struct*)pvPortMalloc(PACKET_SIZE);

    while (1)
    {
        UART6Send((uint8_t *) tx_buf, PACKET_SIZE);
        UARTprintf("sent to uart\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

void COMSocketClientTask(void* pvParameters)
{

//create client socket, configure using setsockopt, bind using bind
    Socket_t xClientSocket[MAX_TASKS];
    socklen_t xSize = sizeof(struct freertos_sockaddr);
    static const TickType_t xTimeOut = pdMS_TO_TICKS(20000); //20 seconds
    int task_num = -1;
    int k = 0;
    uint16_t port = PORT;

    do
    {
        task_num += 1;
        /* Attempt to open the socket. */
        SysCtlDelay(10000);
        xClientSocket[task_num] = FreeRTOS_socket(FREERTOS_AF_INET,
        FREERTOS_SOCK_STREAM,
                                                  FREERTOS_IPPROTO_TCP);
        /* Check the socket was created. */
        configASSERT(xClientSocket[task_num] != FREERTOS_INVALID_SOCKET);
        SysCtlDelay(10000);

        /* Set receive timeout*/
        BaseType_t ret = FreeRTOS_setsockopt(xClientSocket[task_num], 0,
        FREERTOS_SO_RCVTIMEO,
                                             &xTimeOut, sizeof(xTimeOut));
        configASSERT(ret == 0);        //0 for success
        SysCtlDelay(10000);

        /* Set Transmit timeout*/
        ret = FreeRTOS_setsockopt(xClientSocket[task_num], 0,
        FREERTOS_SO_SNDTIMEO,
                                  &xTimeOut, sizeof(xTimeOut));
        configASSERT(ret == 0);        //0 for success
        SysCtlDelay(10000);
        /* Bind the socket, but pass in NULL to let FreeRTOS+TCP choose the port num */
        ret = FreeRTOS_bind(xClientSocket[task_num], NULL, xSize);
        configASSERT(ret == 0);        //0 for success
        UARTprintf("\nClient Created\n");
        SysCtlDelay(10000);
        //set up the remote server address here
        struct freertos_sockaddr xRemoteAddress;
        port += task_num;
        /* Set the IP address & port of server to which client will transmit. */
#if (ipconfigUSE_DHCP == 0 )
        xRemoteAddress.sin_addr = DEFAULT_SERVER_IP_ADDRESS;    //static address
#else
        xRemoteAddress.sin_addr = SERVER_IP_ADDRESS; //gets assigned by the dhcp on router
#endif


        xRemoteAddress.sin_port = FreeRTOS_htons(port);
        SysCtlDelay(10000);
        UARTprintf("connecting to port:%d ", port);
        ret = FreeRTOS_connect(xClientSocket[task_num], &xRemoteAddress,
                               sizeof(xRemoteAddress));
        if (ret != 0)
        {
            UARTprintf("\nClient could not connect to server:%d\n", ret);
            while (1)
            {
                SysCtlDelay(100000);
            }
        }
        SysCtlDelay(10000);
        UARTprintf("\nClient Connected\n");

        if (k == 1)
        {
            UARTprintf("\nCreating ZX TASK\n");
            ret = xTaskCreate(ZXTask, "ZX Task", STACK_DEPTH,
                              (void*) (&xClientSocket[task_num]), 1, &ZXHandle);
            configASSERT(ret == pdPASS);
        }

        else if (k == 0)
        {
            UARTprintf("\nCreating RGB TASK\n");
            ret = xTaskCreate(RGBTask, "RGB Task", STACK_DEPTH,
                              (void*) (&xClientSocket[task_num]), 1,
                              &RGBHandle);
            configASSERT(ret == pdPASS);
            k = 1;

        }

        vTaskDelay(pdMS_TO_TICKS(3000));

    }
    while (task_num < MAX_TASKS - 1);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(60000));
    }        //nothing else to do

}

