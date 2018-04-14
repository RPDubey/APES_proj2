//*****************************************************************************
// File:Main.c
// Implements the requirements for prob4, HW5
// @authorRavi Prakash Dubey
// Date:7th April, 2018
// blinky.c and Uart_echo.c - Examples have been leveraged for this problem
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "FreeRTOS.h" //should be the first to be included from amongst all free rtos files
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "main.h"

#define STACK_DEPTH 1024 //1024 words
#define GPIO_PIN_ON     1
#define GPIO_PIN_OFF    0
#define TICKS_PER_SEC   1500
#define FREQ_2HZ        2
#define FREQ_4HZ        4
#define Q_LENGTH4       4
#define STRING_LENGTH   6
#define MAX_DIGIT_COUNT 10
#define START_INDEX     (MAX_DIGIT_COUNT + 1 - digit_count )
//convert integer to ascii
#define ITOA            digit_count = 0;\
                        quo = count;\
                        do{\
                        rem = quo%10;\
                        quo = (quo - rem)/10;\
                        buffer[MAX_DIGIT_COUNT-digit_count] = rem+48;\
                        digit_count+= 1;\
                        }while(quo);
//                        buffer[MAX_DIGIT_COUNT-digit_count] =' ';


/********************* User Defined Data Types Used *****************************/
typedef enum
{
    TOGGLE_LED = 0x00000001, LOG_STRING = 0x00000002
} eSignalType_t;

typedef struct
{
    char cLogString[STRING_LENGTH];
    uint32_t ulTick_count;
} LogPacket_t;

//Function Definitions
void vTask1(void* pvParameters);
void vTask2(void* pvParameters);
void vTask3(void* pvParameters);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
static void vTimer2hzCallbackFunction(TimerHandle_t xTimer);
static void vTimer4hzCallbackFunction(TimerHandle_t xTimer);

//Global Variables
uint32_t g_ui32SysClock;
TaskHandle_t Task1Handle, Task2Handle, Task3Handle;
QueueHandle_t QHandle;

int main(void)
{

    /***************** Setting up the UART  ****************************/

    // Set the clocking to run directly from the crystal at 120MHz.
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
    SYSCTL_OSC_MAIN |
    SYSCTL_USE_PLL |
    SYSCTL_CFG_VCO_480),
                                            SYSTEM_CLOCK);

    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Set GPIO A0 and A1 as UART pins.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                            UART_CONFIG_PAR_NONE));

    /***************  Creating Que ***************/
    QHandle = xQueueCreate(Q_LENGTH4, sizeof(LogPacket_t));
    configASSERT(QHandle != NULL);
    BaseType_t ret;

    //create Task
    ret = xTaskCreate(vTask1, "Task 2 hz", STACK_DEPTH, NULL, 1, &Task1Handle);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(vTask2, "Task 4 hz", STACK_DEPTH, NULL, 1, &Task2Handle);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(vTask3, "Task 3", STACK_DEPTH, NULL, 1, &Task3Handle);
    configASSERT(ret == pdPASS);

    //start the scheduler
    vTaskStartScheduler();

    for (;;)
        ;

    return 0;
}

/******************   Send a string to the UART.   *************************/
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while (ui32Count--)
    {
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

/***************Timer 1 - 2 HZ Call Back Function***************************/
static void vTimer2hzCallbackFunction(TimerHandle_t xTimer)
{

    BaseType_t ret = xTaskNotify(Task3Handle, TOGGLE_LED, eSetBits);
    configASSERT(ret == pdPASS);
    vTaskResume(Task1Handle);
}

/***************Timer 2 - 4 HZ Call Back Function***************************/
static void vTimer4hzCallbackFunction(TimerHandle_t xTimer)
{
    //put data on QUE here
    LogPacket_t LogItem = { .cLogString = " Tick:", .ulTick_count =
                                    xTaskGetTickCount() };

    BaseType_t ret = xQueueSendToBack(QHandle, (void* )&LogItem, 0);
    configASSERT(ret == pdPASS);

    ret = xTaskNotify(Task3Handle, LOG_STRING, eSetBits);
    configASSERT(ret == pdPASS);
    vTaskResume(Task2Handle);
}

/***************Task3 ***************************/
void vTask3(void* pvParameters)
{

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Check if the peripheral access is enabled.
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    // Enable the GPIO pin for the LED (PF0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    uint32_t led_state = GPIO_PIN_OFF;

    BaseType_t ret;
    uint32_t NotificationVal = 0x00;
    LogPacket_t LogRxd = { .cLogString = "NA", .ulTick_count = 0 }; //filling it with dummy data
    uint32_t quo, count = 0;
    uint32_t rem, digit_count;
    char buffer[11];

    for (;;)
    {

        //BLock on Notification
        ret = xTaskNotifyWait(0xFFFFFFFF, //ulBitsToClearOnEntry if no pending signal - clear all bits
                NotificationVal, //ulBitsToClearOnExit - clear the Notification rxd,
                &NotificationVal,  //pulNotificationValue,
                portMAX_DELAY);    //xTicksToWait
        configASSERT(ret == pdTRUE);

        if (NotificationVal & TOGGLE_LED)
        {
            led_state = !led_state;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, led_state); // Turn on the LED.
        }

        if (NotificationVal & LOG_STRING)
        {
            //read from Que
            xQueueReceive(QHandle, (void*) &LogRxd, 0);
            configASSERT(ret == pdPASS);

            count = ((LogPacket_t*) &LogRxd)->ulTick_count;

            UARTSend((uint8_t *) (((LogPacket_t*) &LogRxd)->cLogString),
                     STRING_LENGTH);
            ITOA
            ;
            UARTSend((uint8_t *) &buffer[START_INDEX], digit_count);

        }

    }

    vTaskDelete(NULL);
}

/***************Task1  - 2 HZ ***************************/
void vTask1(void* pvParameters)
{

    //Create Timer
    TimerHandle_t Timer2hz = xTimerCreate("Timer 2 HZ",
                                          TICKS_PER_SEC/FREQ_2HZ,
                                          pdTRUE,
                                          0, vTimer2hzCallbackFunction);
    configASSERT(Timer2hz != NULL);
    xTimerStart(Timer2hz, 0);

    for (;;)
    {
        vTaskSuspend(Task1Handle);    //resumed in the callback function
    }

    vTaskDelete(NULL);
}

/***************Task2  - 4 HZ ***************************/
void vTask2(void* pvParameters)
{
    //Create Timer
    TimerHandle_t Timer4hz = xTimerCreate("Timer 4 HZ",
                                          TICKS_PER_SEC/FREQ_4HZ,
                                          pdTRUE,
                                          0, vTimer4hzCallbackFunction);
    configASSERT(Timer4hz != NULL);
    xTimerStart(Timer4hz, 0);

    for (;;)
    {
        vTaskSuspend(Task2Handle);    //resumed in the callback function
    }

    vTaskDelete(NULL);
}

