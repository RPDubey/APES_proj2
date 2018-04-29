/*
 * MyServer.c
 *
 *  Created on: Apr 21, 2018
 *      Author: Ravi
 */


/*##########################################################################*/



#include <COM.h>
#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "MyUart.h"
#include "common.h"

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <stdio.h>
#include <string.h>
#include <SensorTasks.h>


void SocketServerTask(void* pvParameters)
{

    //  xSemaphoreTake(xServerSemaphore, portMAX_DELAY  );
    struct freertos_sockaddr xClient, xBindAddress;
    Socket_t xListeningSocket, xConnectedSocket;
    socklen_t xSize = sizeof(xClient);
    static const TickType_t xReceiveTimeOut = portMAX_DELAY;
    const BaseType_t xBacklog = 3;

    /* Attempt to open the socket. */
    xListeningSocket = FreeRTOS_socket( FREERTOS_AF_INET,
                                        FREERTOS_SOCK_STREAM, /* SOCK_STREAM for TCP. */
                                        FREERTOS_IPPROTO_TCP);
    configASSERT(xListeningSocket != FREERTOS_INVALID_SOCKET);

    /* Set a time out so accept() will just wait for a connection. */
    BaseType_t ret = FreeRTOS_setsockopt(xListeningSocket, 0,
                                         FREERTOS_SO_RCVTIMEO,
                                         &xReceiveTimeOut,
                                         sizeof(xReceiveTimeOut));
    configASSERT(ret == 0);        //0 for success

    /* Set the listening port to PORT. */
    xBindAddress.sin_port = (uint16_t) PORT;
    xBindAddress.sin_port = FreeRTOS_htons(xBindAddress.sin_port);

    /* Bind the socket to the port that the client RTOS task will send to. */
    /*When a socket is created it assumes the IP address of the network node that created it.*/
    ret = FreeRTOS_bind(xListeningSocket,
                       (struct freertos_sockaddr *)&xBindAddress,
                       sizeof(xBindAddress));
    SysCtlDelay(1000);
    configASSERT(ret == 0);        //0 for success
    UARTprintf("\nServer Bound to host adddr and port %d\n",PORT);
    /* Set the socket into a listening state so it can accept connections.
     The maximum number of simultaneous connections is limited to 20. */
    ret = FreeRTOS_listen(xListeningSocket, xBacklog);
    SysCtlDelay(1000);
    configASSERT( ret == 0);

    UARTprintf("\nServer Socket Placed in Listening state\n");

    /* Wait for incoming connections. */
      xConnectedSocket = FreeRTOS_accept(xListeningSocket, &xClient, &xSize);
      SysCtlDelay(1000);
      configASSERT(xConnectedSocket != FREERTOS_INVALID_SOCKET);

    UARTprintf("\nAccepted Connection in Server");
    static char cRxedData = 'P';

    while (1)
    {

//   /* Spawn a RTOS task to handle the connection. */
//   xTaskCreate( prvServerConnectionInstance,
//                "EchoServer",
//                usUsedStackSize,
//                ( void * ) xConnectedSocket,
//                tskIDLE_PRIORITY,
//                NULL );
////SysCtlDelay(10000);

        BaseType_t lBytesReceived = FreeRTOS_recv(xConnectedSocket, &cRxedData,
                                                  1, 0);

        if (lBytesReceived > 0)
        {
            UARTprintf("\n Rxd data:%c", &cRxedData);
            /* Data was received, process it here. */
            //  prvPorcessData( cRxedData, lBytesReceived );
        }
        else if (lBytesReceived == 0)
        {
            SysCtlDelay(1000);
        }
        else
        {
            /* Error (maybe the connected socket already shut down the socket?).
             Attempt graceful shutdown. */
            FreeRTOS_shutdown(xConnectedSocket, FREERTOS_SHUT_RDWR);
            SysCtlDelay(10000);
            FreeRTOS_closesocket(xConnectedSocket);
            break;
        }

    }

    while (1)
        ;
}
