/*
 * MyI2C.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "MyI2C.h"
#include "common.h"


extern uint32_t g_ui32SysClock;

#define DATA_RATE_100KBPS   false
#define DATA_RATE_400KBPS   true

void I2C0_Master_Enable()
{

#ifndef TEST
    /***************   I2C **********************/

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    //SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
    //Enable GPIO for Configuring the I2C0 Interface Pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    // Wait for the Peripheral to be ready for programming
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
        ;

    // Configure Pins for I2C0 Master Interface
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

   // I2CLoopbackEnable(I2C0_BASE);     //for debug, loop back mode has been use

    I2CMasterInitExpClk(I2C0_BASE, g_ui32SysClock, DATA_RATE_100KBPS);//enable the master module

#endif

}

void I2C0_Master_Write(uint8_t Slave_Addr,uint8_t DataTx,uint8_t read_reg_addr){

#ifndef TEST
    I2CMasterSlaveAddrSet(I2C0_BASE, Slave_Addr, MASTER_WRITE); //Master to slave SLAVE_ADDR
    I2CMasterDataPut(I2C0_BASE, read_reg_addr);
    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is free
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    SysCtlDelay(1000);
    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is done transferring.
    I2CMasterDataPut(I2C0_BASE, DataTx);
    //while (I2CMasterBusy(I2C0_BASE));// Wait until master module is free
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    SysCtlDelay(1000);
    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is done transferring.
    #endif

}



uint8_t I2C0_Master_Read(uint8_t Slave_Addr, uint8_t read_reg_addr){

#ifndef TEST

    //write the register to read from
    I2CMasterSlaveAddrSet(I2C0_BASE, Slave_Addr, MASTER_WRITE); //Master to slave SLAVE_ADDR
    I2CMasterDataPut(I2C0_BASE, read_reg_addr);
    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is free
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);//I2C_MASTER_CMD_SINGLE_SEND
    SysCtlDelay(1000);
    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is done transferring.

    //read from I2C register
    I2CMasterSlaveAddrSet(I2C0_BASE, Slave_Addr, MASTER_READ);
    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is free
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    SysCtlDelay(1000);
    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is free
    return  (uint8_t)I2CMasterDataGet(I2C0_BASE);

#else
return 84 ;//ascii for T
#endif
}







