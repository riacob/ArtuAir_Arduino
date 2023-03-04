/**
 * @file ds3131.h
 * @authors Riccardo Iacob, Elia Zambelli Franz, Gjovani Negri, Alex Beltrame
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 */
#ifndef DS3231_H
#define DS3231_H

#include <Arduino.h>

class DS3231
{
private:
    uint8_t i2cAdd;

public:
    DS3231(uint8_t i2cAddress);
};

#endif