/*
 * callbacks.c
 *
 *  Created on: Apr 7, 2018
 *      Author: Ravi
 */

#include <COM.h>
#include<stdint.h>
#include<stdlib.h>

#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"
#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include "common.h"

/* Defined by the application code, but called by FreeRTOS when the network
 connects/disconnects (if ipconfigUSE_NETWORK_EVENT_HOOK is set to 1 in
 FreeRTOSIPConfig.h). */
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t eNetworkEvent)
{
    uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
    static BaseType_t xTasksAlreadyCreated = pdFALSE;
    int8_t cBuffer[16];

    /* Check this was a network up event, as opposed to a network down event. */
    if (eNetworkEvent == eNetworkUp)
    {

        /* The network is up and configured.  Print out the configuration,
         which may have been obtained from a DHCP server. */
        FreeRTOS_GetAddressConfiguration(&ulIPAddress, &ulNetMask,
                                         &ulGatewayAddress,
                                         &ulDNSServerAddress);

        /* Convert the IP address to a string then print it out. */
        FreeRTOS_inet_ntoa(ulIPAddress, cBuffer);
        UARTprintf("IP Address: %s\r\n", cBuffer);
        /* Convert the net mask to a string then print it out. */
        FreeRTOS_inet_ntoa(ulNetMask, cBuffer);
        UARTprintf("Subnet Mask: %s\r\n", cBuffer);

        /* Convert the IP address of the gateway to a string then print it out. */
        FreeRTOS_inet_ntoa(ulGatewayAddress, cBuffer);
        UARTprintf("Gateway IP Address: %s\r\n", cBuffer);

        /* Convert the IP address of the DNS server to a string then print it out. */
        FreeRTOS_inet_ntoa(ulDNSServerAddress, cBuffer);
        UARTprintf("DNS server IP Address: %s\r\n", cBuffer);

        /* Create the tasks that use the IP stack if they have not already been
       created. */
       if( xTasksAlreadyCreated == pdFALSE )
       {

           xTasksAlreadyCreated = xTaskCreate(COMSocketClientTask, "Socket Client Task", STACK_DEPTH, NULL, 1,
             NULL);
           configASSERT(xTasksAlreadyCreated == pdPASS);

//           xTasksAlreadyCreated = xTaskCreate(SocketTask, "Socket Task", STACK_DEPTH, NULL, 2,
//                                NULL);
//           configASSERT(xTasksAlreadyCreated == pdPASS);
       }

    }
    else if (eNetworkEvent == eNetworkDown)
    {
        UARTprintf("Network Lost\n");
    }
}


//callback function which takes a character pointer as a parameter and returns pdTRUE if the name
//passed into the function matches a name used to identify the node.
BaseType_t xApplicationDNSQueryHook(const char *pcName)
{
    // return strcasecmp( pcName, "STM32F407" ) == 0 || strcasecmp( pcName, "mydevice" ) == 0;
    return 1;
}




void vMainAssertCalled( const char *pcFileName, uint32_t ulLineNumber )
{
    taskENTER_CRITICAL();
    for( ;; );
}

void vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeHeapSpace;

    /* This is just a trivial example of an idle hook.  It is called on each
    cycle of the idle task.  It must *NOT* attempt to block.  In this case the
    idle task just queries the amount of FreeRTOS heap that remains.  See the
    memory management section on the http://www.FreeRTOS.org web site for memory
    management options.  If there is a lot of heap memory free then the
    configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
    RAM. */
    xFreeHeapSpace = xPortGetFreeHeapSize();

    /* Remove compiler warning about xFreeHeapSpace being set but never used. */
    ( void ) xFreeHeapSpace;
}

void vApplicationTickHook( void )
{
    #if( mainSELECTED_APPLICATION == 1 )
    {
        /* Only the comprehensive demo actually uses the tick hook. */
        extern void vFullDemoTickHook( void );
        vFullDemoTickHook();
    }
    #endif
}
/*-----------------------------------------------------------*/

