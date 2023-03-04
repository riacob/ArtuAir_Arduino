/**
 * @file main.cpp
 * @authors Riccardo Iacob, Elia Zambelli Franz
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "hardware.h"
#include "bme680.h"
#include "ds3231.h"
#include "ssd1306.h"

BME680 bme680(I2C_BME680_ADD);
DS3231 rtc;
SSD1306 oled;

void setup()
{

  // Configure GPIOs
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

  // Begin UART ports
  Serial.begin(BAUDRATE_SERIAL);
  Serial1.begin(BAUDRATE_SERIALBT);

  // Begin OLED, generating 3.3v internally
  if (!oled.begin(SSD1306_SWITCHCAPVCC, I2C_OLED_ADD))
  {
    // Fault code = continuous red led (example)
    digitalWrite(PIN_LED_RED, HIGH);
    // Loop infinitely
    while (1) {}
  }

  // Landing lights pattern on LEDs
  digitalWrite(PIN_LED_GREEN, HIGH);
  delay(150);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YELLOW, HIGH);
  delay(150);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_RED, HIGH);
  delay(150);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_BLUE, HIGH);
  delay(150);
  digitalWrite(PIN_LED_BLUE, LOW);

  // Print welcome screen on oled
  oled.printScreen(SSD1306::Screens::screen_welcome);

}

void loop()
{
  // Wait for data to be available over bluetooth and echo it on serial port
  if (Serial1.available())
  {
    String str = Serial1.readString();
    Serial.println(str);
  }
}