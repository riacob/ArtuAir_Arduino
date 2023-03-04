/**
 * @file ssd1306.h
 * @author Riccardo Iacob
 * @brief 
 * @version 0.1
 * @date 2023-03-04
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef SSD1306_H
#define SSD1306_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

class SSD1306 : public Adafruit_SSD1306
{
    public:
    /**
     * @brief Screen states enumeration
     * Refer to each element's comment to determine what the selected element will print on the display
     */
    enum class Screens {
        /**
         * ArtuAir
         * Malignani Udine
         * 5ELIA A.S. 2022/2023
         */
        screen_welcome
    };
    public:
      SSD1306();
      void printScreen(Screens screen);
};

#endif