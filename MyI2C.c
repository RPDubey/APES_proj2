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

extern uint32_t g_ui32SysClock;


void I2C0_Master_Enable()
{

#ifndef TEST
    /***************   I2C **********************/

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
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

    I2CMasterInitExpClk(I2C0_BASE, g_ui32SysClock, false);//enable the master module

   // I2CSlaveEnable(I2C0_BASE); //enable the slave module
    //I2CSlaveInit(I2C0_BASE, SLAVE_ADDRESS);

#endif

}

void I2C0_Master_Write(uint8_t Slave_Addr,uint8_t pui32DataTx){

#ifndef TEST
//    I2CMasterSlaveAddrSet(I2C0_BASE, Slave_Addr, MASTER_WRITE); //Master to slave SLAVE_ADDR
//    I2CMasterDataPut(I2C0_BASE, pui32DataTx);
//
//    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
//
////    while (!(I2CSlaveStatus(I2C0_BASE) & I2C_SLAVE_ACT_RREQ));//wait for ack from slave
//    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is done transferring.
//
////    pui32DataRx[ui32Index] = I2CSlaveDataGet(I2C0_BASE);
//
//    while (I2CMasterBusy(I2C0_BASE));// Wait until master module is done transferring.
#endif

}



uint8_t I2C0_Master_Read(uint8_t Slave_Addr){

#ifndef TEST
//read from I2C and return
#else
return 84 //ascii for T
#endif
}
