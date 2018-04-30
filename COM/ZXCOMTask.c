#include <COM.h>
#include "utils/uartstdio.h"

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

TaskHandle_t ZXHandle;
TaskHandle_t HBHandle;

extern SemaphoreHandle_t ZX_sem;
extern QueueHandle_t COM_que;
QueueHandle_t ZX_Que;

void ZXTask(void* pvParameters)
{

#ifdef USE_SOCKET
    Socket_t *xClientSocket = (Socket_t*) pvParameters;
#endif
    BaseType_t ret;
    ZX_Que = xQueueCreate(MAX_QUE_SIZE, PACKET_SIZE);
    configASSERT(ZX_Que != NULL);

    /***buffer for tx/rx data*****/
    msg_struct *tx_buf = (msg_struct*) pvPortMalloc(PACKET_SIZE);
    configASSERT(tx_buf != NULL);

//    signal sensor task to proceed and start collecting data
    ret = xSemaphoreGive(ZX_sem);
    configASSERT(ret == pdTRUE)

    while (1)
    {

        //collect data from sensor task
        ret = xQueueReceive(ZX_Que, tx_buf, portMAX_DELAY); //block on receive
        configASSERT(ret == pdPASS);

#ifdef USE_UART
        //send it on que to uart task
        ret = xQueueSendToBack(COM_que, (void * )tx_buf, pdMS_TO_TICKS(3000));
        if (ret != pdPASS)
            UARTprintf("Que Full");
#endif

#ifdef USE_SOCKET
        Socket_t *xClientSocket = (Socket_t*) pvParameters;
        SysCtlDelay(1000);
        BaseType_t xBytesSent = FreeRTOS_send(*xClientSocket, /* The socket being sent to. */
                (void*) (tx_buf),/* The data being sent. */
                PACKET_SIZE,/* The remaining length of data to send. */
                0); /* ulFlags. */
        if (xBytesSent < 0)
        {
            UARTprintf("\nSend Failed:%d", xBytesSent);
        }

#endif
    }
    vTaskDelete(NULL);
}

void HBTask(void* pvParameters)
{
    LED_PF0_SETUP
    uint32_t led_state = GPIO_PIN_OFF;
    while (1)
    {
        vTaskSuspend(HBHandle);
        led_state = !led_state;
        LED_PFO_ON_OFF(led_state);
    }
    vTaskDelete(NULL);
}

