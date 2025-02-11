/*
 * TMP102.h
 *
 *  Created on: May 24, 2024
 *      Author: gert
 */

#ifndef SRC_TMP102_H_
#define SRC_TMP102_H_
#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"
static const uint8_t TMP102_ADDR = 0x48 << 1; // Use 8-bit address

void ScanI2cBus();
bool InitTmp102(I2C_HandleTypeDef *Handle, uint8_t BaseAdress);

bool ReadTemperature(float *temp_c);


#endif /* SRC_TMP102_H_ */
