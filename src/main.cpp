/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>

#include "hardware.h"
#include "bme680.h"
#include "rtc.h"
#include "ssd1306.h"

BME680 bme680;
RTC rtc;
SSD1306 ssd1306;

void setup() {
  // Begin UART port for debugging
  Serial.begin(BAUDRATE);
  // Print welcome screen
  Serial.println("ArtuAir - Air Quality Monitor");
  Serial.println("Malignani Udine");
  Serial.println("5ELIA A.S. 2022/2023");
  Serial.println("Welcome!");
}

void loop() {
}