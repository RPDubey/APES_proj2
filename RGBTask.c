/*
 * RGBTask.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */

#include "MyTasks.h"
#include "utils/uartstdio.h"
#include "driverlib/i2c.h"

#define RGB_SENSOR_ADDRESS 0x44

#define RGB_SENSOR_WRITE(DataTx,reg_addr)  I2C0_Master_Write(RGB_SENSOR_ADDRESS,DataTx,reg_addr)
#define RGB_SENSOR_READ(reg)               I2C0_Master_Read(RGB_SENSOR_ADDRESS,reg)


typedef enum{
    DEVICE_ID = 0x00,
    CONFIG_1 = 0x01,
    CONFIG_2 = 0x02,
    CONFIG_3 = 0x03,
    THRESHOLD_LL = 0x04,
    THRESHOLD_LH = 0x05,
    THRESHOLD_HL = 0x06,
    THRESHOLD_HH = 0x07,
    STATUS = 0x08 ,
    GREEN_L = 0x09 ,
    GREEN_H = 0x0A,
    RED_L = 0x0B,
    RED_H = 0x0C,
    BLUE_L = 0x0D,
    BLUE_H =0x0E
}RGB_SENSOR_REG_t;

// Configuration Settings
#define CFG_DEFAULT 0x00

// CONFIG1
// Pick a mode, determines what color[s] the sensor samples, if any
#define CFG1_MODE_POWERDOWN 0x00
#define CFG1_MODE_G 0x01
#define CFG1_MODE_R 0x02
#define CFG1_MODE_B 0x03
#define CFG1_MODE_STANDBY 0x04
#define CFG1_MODE_RGB 0x05
#define CFG1_MODE_RG 0x06
#define CFG1_MODE_GB 0x07

// Light intensity range
// In a dark environment 375Lux is best, otherwise 10KLux is likely the best option
#define CFG1_375LUX 0x00
#define CFG1_10KLUX 0x08

// Change this to 12 bit if you want less accuracy, but faster sensor reads
// At default 16 bit, each sensor sample for a given color is about ~100ms
#define CFG1_16BIT 0x00
#define CFG1_12BIT 0x10

// Unless you want the interrupt pin to be an input that triggers sensor sampling, leave this on normal
#define CFG1_ADC_SYNC_NORMAL 0x00
#define CFG1_ADC_SYNC_TO_INT 0x20

// CONFIG2
// Selects upper or lower range of IR filtering
#define CFG2_IR_OFFSET_OFF 0x00
#define CFG2_IR_OFFSET_ON 0x80

// Sets amount of IR filtering, can use these presets or any value between 0x00 and 0x3F
// Consult datasheet for detailed IR filtering calibration
#define CFG2_IR_ADJUST_LOW 0x00
#define CFG2_IR_ADJUST_MID 0x20
#define CFG2_IR_ADJUST_HIGH 0x3F

// CONFIG3
// No interrupts, or interrupts based on a selected color
#define CFG3_NO_INT 0x00
#define CFG3_G_INT 0x01
#define CFG3_R_INT 0x02
#define CFG3_B_INT 0x03

// How many times a sensor sample must hit a threshold before triggering an interrupt
// More consecutive samples means more times between interrupts, but less triggers from short transients
#define CFG3_INT_PRST1 0x00
#define CFG3_INT_PRST2 0x04
#define CFG3_INT_PRST4 0x08
#define CFG3_INT_PRST8 0x0C

// If you would rather have interrupts trigger when a sensor sampling is complete, enable this
// If this is disabled, interrupts are based on comparing sensor data to threshold settings
#define CFG3_RGB_CONV_TO_INT_DISABLE 0x00
#define CFG3_RGB_CONV_TO_INT_ENABLE 0x10

// STATUS FLAG MASKS
#define FLAG_INT 0x01
#define FLAG_CONV_DONE 0x02
#define FLAG_BROWNOUT 0x04
#define FLAG_CONV_G 0x10
#define FLAG_CONV_R 0x20
#define FLAG_CONV_B 0x30

void RGBTask(void* pvParameters)
{

    LED_PF0_SETUP
    uint32_t led_state = GPIO_PIN_OFF;
    uint8_t read_val;
    RGB_SENSOR_REG_t reg, reg1, reg2;

//    //reset
//    reg = DEVICE_ID;
//    RGB_SENSOR_WRITE(0x46,reg);




    //configuration
    reg = CONFIG_1;
    RGB_SENSOR_WRITE(CFG1_MODE_RGB | CFG1_10KLUX | CFG1_16BIT | CFG1_ADC_SYNC_NORMAL,reg);
    UARTprintf("\nConfig1:%x",RGB_SENSOR_READ(reg));

    reg = CONFIG_2;
    RGB_SENSOR_WRITE(CFG2_IR_OFFSET_ON | CFG2_IR_ADJUST_HIGH,reg);
    UARTprintf("\nConfig2:%x",RGB_SENSOR_READ(reg));

    reg = CONFIG_3;
    RGB_SENSOR_WRITE(CFG3_NO_INT | CFG3_INT_PRST1 | CFG3_RGB_CONV_TO_INT_DISABLE,reg);
    UARTprintf("\nConfig3:%x",RGB_SENSOR_READ(reg));


    for (;;)
    {
        vTaskSuspend(RGBHandle);
        led_state = !led_state;
        LED_PFO_ON_OFF(led_state);
        UARTprintf("\n");

        read_val = 69;


       reg1 = RED_H; reg2 = RED_L;
       UARTprintf("  RED:%x%x",RGB_SENSOR_READ(reg1),RGB_SENSOR_READ(reg2) );

       reg1 = GREEN_H; reg2 = GREEN_L;
       UARTprintf("  GREEN:%x%x",RGB_SENSOR_READ(reg1),RGB_SENSOR_READ(reg2) );

       reg1 = BLUE_H; reg2 = BLUE_L;
       UARTprintf("  BLUE:%x%x",RGB_SENSOR_READ(reg1),RGB_SENSOR_READ(reg2) );


    }

    vTaskDelete(NULL);
}

