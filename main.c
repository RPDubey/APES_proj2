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

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <stdio.h>
#include <string.h>
#include "MySocket.h"

#define STACK_DEPTH           1024 //1024 words
#define TICKS_PER_SECOND      1000

#define RGB
#define ZX



SemaphoreHandle_t xSemaphore,xServerSemaphore, xClientSemaphore;

void PortLIntHandler(void)
{
    uint32_t read = 0x00;
    //static uint32_t flag = 0;
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

    GPIOInterruptEnable();

    UARTprintf("Start Program\n");
    I2C0_Master_Enable();

    TimerHandle_t Timer2hz;
    Timer2hz = xTimerCreate("Timer 2 HZ", Ticks2hz,
    pdTRUE,
                            0, vTimer2hzCallbackFunction);
    configASSERT(Timer2hz != NULL);
    xTimerStart(Timer2hz, 0);



    /*############################################################################*/
    /* The MAC address array is not declared const as the MAC address will
     normally be read from an EEPROM and not hard coded (in real deployed
     applications).*/
    static uint8_t ucMACAddress[6] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };

    /* Define the network addressing.  These parameters will be used if either
     ipconfigUDE_DHCP is 0 or if ipconfigUSE_DHCP is 1 but DHCP auto configuration
     failed. */
    static const uint8_t ucIPAddress[4] = { 10, 10, 10, 200 };
    static const uint8_t ucNetMask[4] = { 255, 0, 0, 0 };
    static const uint8_t ucGatewayAddress[4] = { 10, 10, 10, 1 };

    /* The following is the address of an OpenDNS server. */
    static const uint8_t ucDNSServerAddress[4] = { 208, 67, 222, 222 };

    /* Initialise the RTOS's TCP/IP stack.  The tasks that use the network
     are created in the vApplicationIPNetworkEventHook() hook function
     below.  The hook function is called when the network connects. */
    BaseType_t ret;
    ret = FreeRTOS_IPInit(ucIPAddress, ucNetMask,
                          ucGatewayAddress,ucDNSServerAddress, ucMACAddress);
    configASSERT(ret == pdPASS);
    SysCtlDelay(1000);
    UARTprintf("Initialized TCP/IP Stack\n");

//    ret = xTaskCreate(SocketServerTask, "Server Task", STACK_DEPTH, NULL, 1,
//                       NULL);
//    configASSERT(ret == pdPASS);
//
//    SysCtlDelay(10000);

//    ret = xTaskCreate(SocketClientTask, "Client Task", STACK_DEPTH, NULL, 1,
//                       NULL);
//    configASSERT(ret == pdPASS);

    //#ifdef ZX
//    ret = xTaskCreate(ZXTask, "ZX Task", STACK_DEPTH, NULL, 2, &ZXHandle);
//    configASSERT(ret == pdPASS);
//#endif

#ifdef RGB
    ret = xTaskCreate(RGBTask, "RGB Task", STACK_DEPTH, NULL, 2, &RGBHandle);
    configASSERT(ret == pdPASS);
#endif

    vTaskStartScheduler();

    while (1)
        ;
    return 0;
}


