/**
 * @file ds3131.cpp
 * @authors Riccardo Iacob, Elia Zambelli Franz, Gjovani Negri, Alex Beltrame
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 */
#include "ds3231.h"

DS3231::DS3231(uint8_t i2cAddress)
{
    i2cAdd = i2cAddress;
}