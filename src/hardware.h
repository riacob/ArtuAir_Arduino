/**
 * @file hardware.h
 * @authors Riccardo Iacob, Elia Zambelli Franz
 * @brief 
 * @version 0.1
 * @date 2023-02-27
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef HARDWARE_H
#define HARDWARE_H

#define BAUDRATE_SERIAL 115200
#define BAUDRATE_SERIALBT 38400

#define I2C_DS3231_ADD 0x68
#define I2C_BME680_ADD 0x77
#define I2C_OLED_ADD 0x3C
#define I2C_EEPROM_ADD 0x57

#define PIN_LED_GREEN 52
#define PIN_LED_YELLOW 51 
#define PIN_LED_RED 49 
#define PIN_LED_BLUE 47

#endif