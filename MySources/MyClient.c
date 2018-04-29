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

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <stdio.h>
#include <string.h>

#include "MySocket.h"
#include "MyTasks.h"
#include "MyUart.h"
#include "common.h"


extern SemaphoreHandle_t xSemaphore ;

//Global Variables
extern TaskHandle_t RGBHandle, ZXHandle;
TaskHandle_t  HBHandle;


void COMSocketClientTask(void* pvParameters)
{

//create client socket, configure using setsockopt, bind using bind
    Socket_t xClientSocket[MAX_TASKS];
    socklen_t xSize = sizeof(struct freertos_sockaddr);
    static const TickType_t xTimeOut = pdMS_TO_TICKS(20000);//20 seconds
    int task_num = -1;
    int k =0;
    uint16_t port = PORT;


    do{
    task_num+=1;
    /* Attempt to open the socket. */
    xClientSocket[task_num] = FreeRTOS_socket(FREERTOS_AF_INET,
                                    FREERTOS_SOCK_STREAM,
                                    FREERTOS_IPPROTO_TCP);
    /* Check the socket was created. */
    configASSERT(xClientSocket[task_num] != FREERTOS_INVALID_SOCKET);
    /* Set receive timeout*/
    BaseType_t ret = FreeRTOS_setsockopt(xClientSocket[task_num], 0,
                                         FREERTOS_SO_RCVTIMEO,
                                         &xTimeOut, sizeof(xTimeOut));
    configASSERT(ret == 0);        //0 for success
    /* Set Transmit timeout*/
    ret = FreeRTOS_setsockopt(xClientSocket[task_num], 0,
                              FREERTOS_SO_SNDTIMEO,
                              &xTimeOut, sizeof(xTimeOut));
    configASSERT(ret == 0);        //0 for success
    SysCtlDelay(1000);
    /* Bind the socket, but pass in NULL to let FreeRTOS+TCP choose the port num */
    ret = FreeRTOS_bind(xClientSocket[task_num], NULL, xSize);
    configASSERT(ret == 0);        //0 for success
    UARTprintf("\nClient Created\n");
    SysCtlDelay(1000);
    //set up the remote server address here
    struct freertos_sockaddr xRemoteAddress;
    port += task_num;
    /* Set the IP address & port of server to which client will transmit. */
    UARTprintf("port:%d ",port);
    xRemoteAddress.sin_port = FreeRTOS_htons(port);

    #if (ipconfigUSE_DHCP == 0 )
        xRemoteAddress.sin_addr = DEFAULT_SERVER_IP_ADDRESS;//static address
    #else
        xRemoteAddress.sin_addr = SERVER_IP_ADDRESS;//gets assigned by the dhcp on router
    #endif

    ret = FreeRTOS_connect(xClientSocket[task_num], &xRemoteAddress,sizeof(xRemoteAddress));
    if(ret!=0)
    {
        UARTprintf("\nClient could not connect to server:%d\n",ret);
        while(1){SysCtlDelay(10000);}
    }
    SysCtlDelay(10000);
    UARTprintf("\nClient Connected\n");

if(k == 1){
    UARTprintf("\nCreating ZX TASK\n");
    ret = xTaskCreate(ZXTask, "ZX Task", STACK_DEPTH, (void*)(&xClientSocket[task_num]), 1, &ZXHandle);
    configASSERT(ret == pdPASS);
}

else if(k == 0){
    UARTprintf("\nCreating HB TASK\n");
    ret = xTaskCreate(HBTask, "HB Task", STACK_DEPTH, (void*)(&xClientSocket[task_num]), 1, &HBHandle);
       configASSERT(ret == pdPASS);
       k=1;

}

vTaskDelay(pdMS_TO_TICKS(3000));

}while(task_num < MAX_TASKS - 1);

while (1){vTaskDelay(pdMS_TO_TICKS(60000));}//nothing else to do

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
