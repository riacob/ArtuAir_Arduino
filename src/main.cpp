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
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "hardware.h"
#include "bme680.h"
#include "rtc.h"

BME680 bme680(I2C_BME680_ADD);
RTC rtc;
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void setup()
{
  // Begin UART ports
  Serial.begin(BAUDRATE_SERIAL);
  Serial1.begin(BAUDRATE_SERIALBT);

  // Begin OLED, generating 3.3v internally
  if (!oled.begin(SSD1306_SWITCHCAPVCC, I2C_OLED_ADD))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Print welcome screen
  Serial.println("ArtuAir - Air Quality Monitor");
  Serial.println("Malignani Udine");
  Serial.println("5ELIA A.S. 2022/2023");
  Serial.println("Welcome!");
}

void loop()
{
  if (Serial1.available())
  {
    String str = Serial1.readString();
    Serial.println(str);
  }
}