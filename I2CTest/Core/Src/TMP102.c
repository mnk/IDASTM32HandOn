/*
 * TMP102.c
 *
 *  Created on: May 24, 2024
 *      Author: gert
 */
#include "TMP102.h"
extern I2C_HandleTypeDef hi2c1;
static const uint8_t REG_TEMP = 0x00;


I2C_HandleTypeDef *defaultHandle;
uint8_t DefaultBaseAdress;
uint8_t buf[12];
bool I2cBusActive[128]; //only for debugging

bool InitTmp102(I2C_HandleTypeDef *Handle, uint8_t BaseAdress) {
	HAL_StatusTypeDef ret;
	DefaultBaseAdress =	BaseAdress;
	defaultHandle     = Handle;
    buf[0] = REG_TEMP;
    ret = HAL_I2C_Master_Transmit(defaultHandle, DefaultBaseAdress, buf, 1, HAL_MAX_DELAY);
	return (ret = HAL_OK);
}

bool ReadTemperature(float *temp_c) {
	HAL_StatusTypeDef ret;
	int16_t val;
	// Tell TMP102 that we want to read from the temperature register
    buf[0] = REG_TEMP;
    ret = HAL_I2C_Master_Transmit(defaultHandle, DefaultBaseAdress, buf, 1, HAL_MAX_DELAY);
    if ( ret == HAL_OK) {
    	ret = HAL_I2C_Master_Receive(defaultHandle, DefaultBaseAdress, buf, 2, HAL_MAX_DELAY);
        if ( ret == HAL_OK) {
            //Combine the bytes
            val = ((int16_t)buf[0] << 4) | (buf[1] >> 4);

            // Convert to 2's complement, since temperature can be negative
            if ( val > 0x7FF ) {
              val |= 0xF000;
            }

            // Convert to float temperature value (Celsius)
            *temp_c = val * 0.0625;

        }

    }
  return (ret = HAL_OK);
}

void ScanI2cBus(){
	HAL_StatusTypeDef ret;
    for(uint8_t i=1; i<128; i++)
    {
        ret = HAL_I2C_IsDeviceReady(defaultHandle, (uint16_t)(i<<1), 3, 5);
        if (ret != HAL_OK) /* No ACK Received At That Address */
        {

        }
        else if(ret == HAL_OK)
        {
            I2cBusActive[i-1]=true;
        }
    }
    /*--[ Scanning Done ]--*/
}
