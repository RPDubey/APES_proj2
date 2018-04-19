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


#define RGB
//#define ZX


void PortCIntHandler(void){
    uint32_t read = 0x00;
    static uint32_t flag =0;


    read = GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3);

    if(!read)
{
    UARTprintf("\nHello World: %X",read);
    SysCtlDelay(1000);//debounce
}

    GPIOIntClear(GPIO_PORTL_BASE, GPIO_INT_PIN_3);

}



    GPIOInterruptEnable(){
        //int32_t i32Val;
        //
        // Enable the GPIOA peripheral
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
        //
        // Wait for the GPIOA module to be ready.
        //
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
        //
        // Register the port-level interrupt handler. This handler is the first
        // level interrupt handler for all the pin interrupts.
        //
        GPIOIntRegister(GPIO_PORTL_BASE, PortCIntHandler);
        //
        // Initialize the GPIO pin configuration.
        //
        // Set pins 2, 4, and 5 as input, SW controlled.
        //
        GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_3);
//        //
//        // Set pins 0 and 3 as output, SW controlled.
//        //
//        GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);
//        //
//        // Make pins 2 and 4 rising edge triggered interrupts.
//        //
//        GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4, GPIO_RISING_EDGE);
        //
        // Make pin 7 high level triggered interrupts.
        //
        GPIOIntTypeSet(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_FALLING_EDGE);
        //
        // Read some pins.
        //

//        i32Val = GPIOPinRead(GPIO_PORTA_BASE,
//        (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 |
//        GPIO_PIN_4 | GPIO_PIN_5));
//        //
//        // Write some pins. Even though pins 2, 4, and 5 are specified, those pins
//        // are unaffected by this write because they are configured as inputs. At
//        // the end of this write, pin 0 is low, and pin 3 is high.
//        //
 //       GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 1);

        //
        // Enable the pin interrupts.
        //

        GPIOIntEnable(GPIO_PORTL_BASE, GPIO_PIN_3 );



    }



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

    GPIOInterruptEnable();



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

