/**
 * @file bme680.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "bme680.h"

// definition of the addresses of the sensor-s registers
/*
#define ADD_STATUS              0x73
#define ADD_RESET               0xE0
#define ADD_ID                  0xD0
#define ADD_CONFIG              0x75
#define ADD_CTRL_MEAS           0x74
#define ADD_CTRL_HUM            0x72
#define ADD_CTRL_GAS_1          0x71
#define ADD_CTRL_GAS_0          0x70
#define ADD_GAS_WAIT_X_FIRST    0x6D
#define ADD_GAS_WAIT_X_LAST     0x64
#define ADD_RES_HEAT_X_FIRST    0x63
#define ADD_RES_HEAT_X_LAST     0x5A
#define ADD_IDAC_HEAT_X_FIRST   0x59
#define ADD_IDAC_HEAT_X_LAST    0x50
#define ADD_GAS_R_LSB           0x2B
#define ADD_GAS_R_MSB           0x2A
#define ADD_HUM_LSB             0x26
#define ADD_HUM_MSB             0x25
#define ADD_TEMP_XLSB           0x24
#define ADD_TEMP_LSB            0x23
#define ADD_TEMP_MSB            0x22
#define ADD_PRESS_XLSB          0x21
#define ADD_PRESS_LSB           0x20
#define ADD_PRESS_MSB           0x1F
#define ADD_EAS_STARUS_0        0x1D
*/

BME680::BME680(uint8_t i2cAddress){
    i2cAdd = i2cAddress;
}

