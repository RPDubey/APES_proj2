/*
 * MyI2C.h
 *
 *  Created on: Apr 13, 2018
 *      Author: Ravi
 */

#ifndef MYI2C_H_
#define MYI2C_H_

#include <stdint.h>

#define MASTER_WRITE          0
#define MASTER_READ           1


void I2C0_Master_Enable();

uint8_t I2C0_Master_Read(uint8_t Slave_Addr);

void I2C0_Master_Write(uint8_t Slave_Addr,uint8_t DataTx);

#endif /* MYI2C_H_ */
