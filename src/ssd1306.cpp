/**
 * @file ssd1306.cpp
 * @author Riccardo Iacob
 * @brief
 * @version 0.1
 * @date 2023-03-04
 *
 * @copyright Copyright (c) 2023
 */

#include "ssd1306.h"

SSD1306::SSD1306() : Adafruit_SSD1306(128, 64, &Wire, -1)
{
}

void SSD1306::printScreen(Screens screen)
{
    switch (screen)
    {
    case Screens::screen_welcome:
    {
        clearDisplay();
        setTextColor(SSD1306_WHITE);
        setTextSize(3);
        println("ArtuAir");
        setTextSize(1);
        println();
        println();
        println("   Malignani Udine");
        println(" 5ELIA A.S. 2022/2023");
        display();
    }
    break;

    default:
    {
        break;
    }
    }
}