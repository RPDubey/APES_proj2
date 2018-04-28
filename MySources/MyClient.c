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
void COMSocketClientTask(void* pvParameters)
{
    // xSemaphoreTake(xClientSemaphore, portMAX_DELAY  );

//create client socket, configure using setsockopt, bind using bind
    Socket_t xClientSocket;
    socklen_t xSize = sizeof(struct freertos_sockaddr);
    static const TickType_t xTimeOut = pdMS_TO_TICKS(20000);//20 seconds

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
    ret = FreeRTOS_bind(xClientSocket, NULL, xSize);
    configASSERT(ret == 0);        //0 for success
UARTprintf("\nClient Created and bound to port\n");
    //set up the remote server address here
    struct freertos_sockaddr xRemoteAddress;
    /* Set the IP address & port of server to which client will transmit. */
    xRemoteAddress.sin_port = FreeRTOS_htons(PORT);

#if (ipconfigUSE_DHCP == 0 )
    xRemoteAddress.sin_addr = FreeRTOS_inet_addr_quick(10,10,10,200);//static address
#else
    xRemoteAddress.sin_addr = FreeRTOS_inet_addr_quick(192,168,1,15);//gets assigned by the dhcp on routerFreeRTOS_inet_addr_quick(192,168,1,13)
#endif

    ret = FreeRTOS_connect(xClientSocket, &xRemoteAddress,
                           sizeof(xRemoteAddress));

    //    configASSERT(ret == 0);        //0 for success
if(ret!=0)
{
    UARTprintf("\nClient could not connect to server:%d\n",ret);
    while(1){SysCtlDelay(10000);}
}

    msg_struct *tx_buf= (msg_struct*)pvPortMalloc(PACKET_SIZE);
    configASSERT(tx_buf != NULL);
    strcpy(tx_buf->message, "Hello saranya 123456");


    msg_struct *rx_buf= (msg_struct*)pvPortMalloc(PACKET_SIZE);
    configASSERT(rx_buf != NULL);

            //{.dev_ID=1,.msg_type=com,.message="12345678998765432112"};

    while (1)
    {

      BaseType_t xBytesSent = FreeRTOS_send(xClientSocket, /* The socket being sent to. */
                                   (void*) (tx_buf),/* The data being sent. */
                                   PACKET_SIZE,/* The remaining length of data to send. */
                                   0); /* ulFlags. */
        if (xBytesSent >= 0)
            UARTprintf("\nBytes Sent:%d", xBytesSent);
        else
            break;

xBytesSent = -1;
  // read from server
         xBytesSent = FreeRTOS_recv(xClientSocket,(void*)(rx_buf),PACKET_SIZE,0);

         if (xBytesSent >= 0){
            UARTprintf("\nBytes Rxd:%d", xBytesSent);
            UARTprintf("\nclient:dev_ID:%d",((msg_struct*)rx_buf)->dev_ID);
            UARTprintf("\nclient:msg_type:%d",((msg_struct*)rx_buf)->msg_type);

         }

         //UARTprintf("\nclient:msg_type:%s\n",((msg_struct*)rx_buf)->message);


//         UARTprintf("\nread in child:%d,%d,%s\n",((msg_struct*)message_buf)->dev_ID,
//                                                 ((msg_struct*)message_buf)->msg_type,
//                                                 ((msg_struct*)message_buf)->message );


         //if( ((msg_struct*)message_buf)->msg_type == com){




        vTaskDelay(pdMS_TO_TICKS(1000));

    }

    /* Initiate graceful shutdown. */
    FreeRTOS_shutdown(xClientSocket, FREERTOS_SHUT_RDWR);

    /* Wait for the socket to disconnect gracefully (indicated by FreeRTOS_recv()
     returning a FREERTOS_EINVAL error) before closing the socket. */
//    while (FreeRTOS_recv(xClientSocket, (void*) &pcBufferToTransmit, 1, 0) >= 0)
//    {
//        /* Wait for shutdown to complete.  If a receive block time is used then
//         this delay will not be necessary as FreeRTOS_recv() will place the RTOS task
//         into the Blocked state anyway. */
//        vTaskDelay(pdMS_TO_TICKS(250));
//
//        /* Note - real applications should implement a timeout here, not just
//         loop forever. */
//    }

    while (1)
    {
    }

}

/****called from the hook function once IP is assigned****/
void SocketTask(void* pvParameters)
{
      BaseType_t ret;
    ret = xTaskCreate(SocketServerTask, "Server Task", STACK_DEPTH, NULL, 2,
    NULL);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(COMSocketClientTask, "Client Task", STACK_DEPTH, NULL, 1,
    NULL);
    configASSERT(ret == pdPASS);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
