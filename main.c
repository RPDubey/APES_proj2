//*****************************************************************************
// File:Main.c
// Implements TIVA client for Proj2
// @authorRavi Prakash Dubey
// Date:10th April, 2018
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom_map.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "MyI2C.h"

#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "MyTasks.h"
#include "MyUart.h"
#include "common.h"

#define STACK_DEPTH           1024 //1024 words
#define TICKS_PER_SECOND      1000

#define RGB
#define ZX

void PortLIntHandler(void)
{
    uint32_t read = 0x00;
    static uint32_t flag = 0;
    read = GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3);

    if (!read)
    {
        UARTprintf("\nRED:%x,%x", RGB_SENSOR_READ(RED_H),
                   RGB_SENSOR_READ(RED_L));
    }

#ifdef ZX
    xSemaphoreGiveFromISR(xSemaphore, pdFAIL);
#endif

    GPIOIntClear(GPIO_PORTL_BASE, GPIO_INT_PIN_3);

}

GPIOInterruptEnable()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL))
        ;
    GPIOIntRegister(GPIO_PORTL_BASE, PortLIntHandler);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_3);
    GPIOIntTypeSet(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTL_BASE, GPIO_PIN_3);
}

uint32_t g_ui32SysClock;
static void vTimer2hzCallbackFunction(TimerHandle_t xTimer)
{

#ifdef RGB
    vTaskResume(RGBHandle);
#endif

}

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <stdio.h>
#include <string.h>

/* Defined by the application code, but called by FreeRTOS+UDP when the network
connects/disconnects (if ipconfigUSE_NETWORK_EVENT_HOOK is set to 1 in
FreeRTOSIPConfig.h). */

void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
//static BaseType_t xTasksAlreadyCreated = pdFALSE;
int8_t cBuffer[ 16 ];

    /* Check this was a network up event, as opposed to a network down event. */
    if( eNetworkEvent == eNetworkUp )
    {
//        /* Create the tasks that use the IP stack if they have not already been
//        created. */
//        if( xTasksAlreadyCreated == pdFALSE )
//        {
//            /*
//             * Create the tasks here.
//             */
//
//            xTasksAlreadyCreated = pdTRUE;
//        }

        /* The network is up and configured.  Print out the configuration,
        which may have been obtained from a DHCP server. */
        FreeRTOS_GetAddressConfiguration( &ulIPAddress,
                                          &ulNetMask,
                                          &ulGatewayAddress,
                                          &ulDNSServerAddress );

        /* Convert the IP address to a string then print it out. */
        FreeRTOS_inet_ntoa( ulIPAddress, cBuffer );
        UARTprintf( "IP Address: %s\r\n", cBuffer );

        /* Convert the net mask to a string then print it out. */
        FreeRTOS_inet_ntoa( ulNetMask, cBuffer );
        UARTprintf( "Subnet Mask: %s\r\n", cBuffer );

        /* Convert the IP address of the gateway to a string then print it out. */
        FreeRTOS_inet_ntoa( ulGatewayAddress, cBuffer );
        UARTprintf( "Gateway IP Address: %s\r\n", cBuffer );

        /* Convert the IP address of the DNS server to a string then print it out. */
        FreeRTOS_inet_ntoa( ulDNSServerAddress, cBuffer );
        UARTprintf( "DNS server IP Address: %s\r\n", cBuffer );
    }
}


//callback function which takes a character pointer as a parameter and returns pdTRUE if the name
//passed into the function matches a name used to identify the node.
BaseType_t xApplicationDNSQueryHook( const char *pcName )
{
   // return strcasecmp( pcName, "STM32F407" ) == 0 || strcasecmp( pcName, "mydevice" ) == 0;
    return 1;
}

int main(void)
{
    //set clock frequency to 120 MHZ
    g_ui32SysClock = MAP_SysCtlClockFreqSet(
            (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL
                    | SYSCTL_CFG_VCO_480),
            SYSTEM_CLOCK);

    static const uint32_t ulFrequency2hz = 2;
    TickType_t Ticks2hz = pdMS_TO_TICKS(TICKS_PER_SECOND/ulFrequency2hz);

    UART0Enable();
    UARTStdioConfig(0, 115200, g_ui32SysClock);

//    GPIOInterruptEnable();

//initialize binary semaphore
//    xSemaphore = xSemaphoreCreateBinary();
//    configASSERT(xSemaphore != NULL);
//
//    I2C0_Master_Enable();
//
    TimerHandle_t Timer2hz;
    Timer2hz = xTimerCreate("Timer 2 HZ", Ticks2hz,
    pdTRUE,
                            0, vTimer2hzCallbackFunction);
    configASSERT(Timer2hz != NULL);
    xTimerStart(Timer2hz, 0);

    UARTprintf("\nStart Program");

    //create Task
    BaseType_t ret;

/*##########################################################################*/
//    /* The MAC address array is not declared const as the MAC address will
//    normally be read from an EEPROM and not hard coded (in real deployed
//    applications).*/
//    static uint8_t ucMACAddress[ 6 ] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
//
//    /* Define the network addressing.  These parameters will be used if either
//    ipconfigUDE_DHCP is 0 or if ipconfigUSE_DHCP is 1 but DHCP auto configuration
//    failed. */
//    static const uint8_t ucIPAddress[ 4 ] = { 10, 10, 10, 200 };
//    static const uint8_t ucNetMask[ 4 ] = { 255, 0, 0, 0 };
//    static const uint8_t ucGatewayAddress[ 4 ] = { 10, 10, 10, 1 };
//
//    /* The following is the address of an OpenDNS server. */
//    static const uint8_t ucDNSServerAddress[ 4 ] = { 208, 67, 222, 222 };
//
//        /* Initialise the RTOS's TCP/IP stack.  The tasks that use the network
//        are created in the vApplicationIPNetworkEventHook() hook function
//        below.  The hook function is called when the network connects. */
//        FreeRTOS_IPInit( ucIPAddress,
//                         ucNetMask,
//                         ucGatewayAddress,
//                         ucDNSServerAddress,
//                         ucMACAddress );
//
//
/*##########################################################################*/

    //#ifdef ZX
//    ret = xTaskCreate(ZXTask, "ZX Task", STACK_DEPTH, NULL, 2, &ZXHandle);
//    configASSERT(ret == pdPASS);
//#endif

#ifdef RGB
    ret = xTaskCreate(RGBTask, "RGB Task", STACK_DEPTH, NULL, 1, &RGBHandle);
    configASSERT(ret == pdPASS);
#endif

    vTaskStartScheduler();

    while (1)
        ;
    return 0;
}

