//*****************************************************************************
// File:Main.c
// Implements TIVA client for Proj2
// @authorRavi Prakash Dubey
// Date:10th April, 2018
//
//*****************************************************************************

#include <COM.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
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

#include "MyUart.h"
#include "common.h"

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <stdio.h>
#include <string.h>
#include <SensorTasks.h>

#define STACK_DEPTH           1024 //1024 words
#define TICKS_PER_SECOND      1000

#define RGB
#define ZX


extern TaskHandle_t  HBHandle, ZXSensorHandle;
BaseType_t xHighPriorityWoken = pdFALSE;

SemaphoreHandle_t xSemaphore,xServerSemaphore, xClientSemaphore;
SemaphoreHandle_t ZX_sem, RGB_sem ;

uint8_t HB_flag;
void PortLIntHandler(void)
{
    HB_flag = 0;
    uint32_t read = 0x00;
    read = GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3);
    vTaskNotifyGiveFromISR(ZXSensorHandle, &xHighPriorityWoken);
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
    IntPrioritySet(INT_GPIOL_TM4C129, 0xE0);
}

uint32_t g_ui32SysClock;
static void vTimer2hzCallbackFunction(TimerHandle_t xTimer)
{

vTaskResume(HBHandle);
vTaskNotifyGiveFromISR(ZXSensorHandle, &xHighPriorityWoken);
}


int main(void)
{
HB_flag = 1;
    //set clock frequency to 120 MHZ
    g_ui32SysClock = MAP_SysCtlClockFreqSet(
            (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL
                    | SYSCTL_CFG_VCO_480),
            SYSTEM_CLOCK);

    static const uint32_t ulFrequency2hz = 2;
    TickType_t Ticks2hz = pdMS_TO_TICKS(2000);

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
    BaseType_t ret;

    /************SEM*****/
    ZX_sem = xSemaphoreCreateBinary();
    configASSERT(ZX_sem != NULL)

    RGB_sem = xSemaphoreCreateBinary();
    configASSERT(RGB_sem != NULL)

#ifdef USE_SOCKET
    static uint8_t ucMACAddress[6] = { 0x00, 0x1A, 0xB6, 0x03, 0x2E, 0x0F };
    static const uint8_t ucIPAddress[4] = { 192, 168, 0, 35 };
    static const uint8_t ucNetMask[4] = { 255, 255, 255, 0 };
    static const uint8_t ucGatewayAddress[4] = { 192, 168, 0, 1 };
    static const uint8_t ucDNSServerAddress[4] = { 192, 168, 0, 1 };

    /* Initialise the RTOS's TCP/IP stack.*/
    ret = FreeRTOS_IPInit(ucIPAddress, ucNetMask,ucGatewayAddress,
                          ucDNSServerAddress, ucMACAddress);
    configASSERT(ret == pdPASS);
    SysCtlDelay(10000);
    UARTprintf("Initialized TCP/IP Stack\n");
#endif

#ifdef USE_UART
    ret = xTaskCreate(COMUARTClientTask, "UART Client Task",
                      STACK_DEPTH, NULL, 1,NULL);
    configASSERT(ret == pdPASS);

#endif


    ret = xTaskCreate(HBTask, "HB Task", STACK_DEPTH, NULL, 1, &HBHandle);
    configASSERT(ret == pdPASS);
    SysCtlDelay(1000);
    ret = xTaskCreate(ZXSensorTask, "ZX Sensor Task", STACK_DEPTH, NULL,  1, &ZXSensorHandle);//configMAX_PRIORITIES -
    configASSERT(ret == pdPASS);
    SysCtlDelay(10000);
    ret = xTaskCreate(RGBSensorTask, "RGB Sensor Task", STACK_DEPTH, NULL, 1, NULL);
    configASSERT(ret == pdPASS);
    SysCtlDelay(30000);

    vTaskStartScheduler();

    while (1)
        ;

}


