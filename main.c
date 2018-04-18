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

#include "MyTasks.h"
#include "MyUart.h"
#include "common.h"


#define STACK_DEPTH           1024 //1024 words
#define TICKS_PER_SECOND      1000


//#define RGB
#define ZX

uint32_t g_ui32SysClock;

static void vTimer2hzCallbackFunction(TimerHandle_t xTimer)
{
#ifdef ZX
    vTaskResume(ZXHandle);
#endif

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

    I2C0_Master_Enable();

    TimerHandle_t Timer2hz;
    Timer2hz = xTimerCreate("Timer 2 HZ", Ticks2hz,
    pdTRUE,
                            0, vTimer2hzCallbackFunction);
    configASSERT(Timer2hz != NULL);
    xTimerStart(Timer2hz, 0);



    UARTprintf("\nStart Program");


    //create Task
    BaseType_t ret;

#ifdef RGB
    ret = xTaskCreate(RGBTask, "RGB Task", STACK_DEPTH, NULL, 1, &RGBHandle);
    configASSERT(ret == pdPASS);
#endif

#ifdef ZX
    ret = xTaskCreate(ZXTask, "ZX Task", STACK_DEPTH, NULL, 1, &ZXHandle);
       configASSERT(ret == pdPASS);
#endif

    vTaskStartScheduler();


    while (1)
        ;
    return 0;
}

