/*
 * MyClient.c
 *
 *  Created on: Apr 21, 2018
 *      Author: Ravi
 */

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

#include "MySocket.h"


/*##########################################################################*/
void SocketClientTask(void* pvParameters)
{
    // xSemaphoreTake(xClientSemaphore, portMAX_DELAY  );

//create client socket, configure using setsockopt, bind using bind
    Socket_t xClientSocket;
    socklen_t xSize = sizeof(struct freertos_sockaddr);
    static const TickType_t xTimeOut = pdMS_TO_TICKS(20000);

    /* Attempt to open the socket. */
    xClientSocket = FreeRTOS_socket(FREERTOS_AF_INET,
                                    FREERTOS_SOCK_STREAM,
                                    FREERTOS_IPPROTO_TCP);
   /* Check the socket was created. */
    configASSERT(xClientSocket != FREERTOS_INVALID_SOCKET);
   /* Set receive timeout*/
    BaseType_t ret = FreeRTOS_setsockopt(xClientSocket, 0,
                                         FREERTOS_SO_RCVTIMEO,
                                         &xTimeOut, sizeof(xTimeOut));
    configASSERT(ret == 0);        //0 for success
    /* Set Transmit timeout*/
    ret = FreeRTOS_setsockopt(xClientSocket, 0,
                                         FREERTOS_SO_SNDTIMEO,
                                         &xTimeOut, sizeof(xTimeOut));
    configASSERT(ret == 0);        //0 for success

    /* Bind the socket, but pass in NULL to let FreeRTOS+TCP choose the port num */
    ret=FreeRTOS_bind(xClientSocket, NULL, xSize);
    configASSERT(ret == 0);        //0 for success

    //set up the remote server address here
    struct freertos_sockaddr xRemoteAddress;
    /* Set the IP address & port of server to which client will transmit. */
    xRemoteAddress.sin_port = FreeRTOS_htons(PORT);
    xRemoteAddress.sin_addr = FreeRTOS_inet_addr_quick(192, 168, 1 ,3);


    ret=FreeRTOS_connect(xClientSocket, &xRemoteAddress, sizeof(xRemoteAddress));
    configASSERT(ret == 0);        //0 for success

    BaseType_t xBytesSent;
    const char pcBufferToTransmit = 'T';
    while (1)
    {

        xBytesSent = FreeRTOS_send(xClientSocket, /* The socket being sent to. */
                                  (void*) (&pcBufferToTransmit),/* The data being sent. */
                                  sizeof(pcBufferToTransmit),/* The remaining length of data to send. */
                                  0); /* ulFlags. */
        if (xBytesSent >= 0)      UARTprintf("\nBytes Sent:%d", xBytesSent);
        else break;

        vTaskDelay(pdMS_TO_TICKS(1000));

    }

    /* Initiate graceful shutdown. */
    FreeRTOS_shutdown(xClientSocket, FREERTOS_SHUT_RDWR);

    /* Wait for the socket to disconnect gracefully (indicated by FreeRTOS_recv()
     returning a FREERTOS_EINVAL error) before closing the socket. */
    while (FreeRTOS_recv(xClientSocket, (void*) &pcBufferToTransmit, 1, 0) >= 0)
    {
        /* Wait for shutdown to complete.  If a receive block time is used then
         this delay will not be necessary as FreeRTOS_recv() will place the RTOS task
         into the Blocked state anyway. */
        vTaskDelay(pdMS_TO_TICKS(250));

        /* Note - real applications should implement a timeout here, not just
         loop forever. */
    }

    while (1)
        ;

}


void SocketTask(void* pvParameters)
{
    BaseType_t ret;
    ret = xTaskCreate(SocketServerTask, "Server Task", STACK_DEPTH, NULL, 2,
                      NULL);
    configASSERT(ret == pdPASS);
   // UARTprintf("\nServer Task created ");


    ret = xTaskCreate(SocketClientTask, "Client Task", STACK_DEPTH, NULL, 1,
                      NULL);
    configASSERT(ret == pdPASS);
//    UARTprintf("\nClient Task created");



    while(1)
    {
        vTaskDelay( pdMS_TO_TICKS(2000) );
    }
}
