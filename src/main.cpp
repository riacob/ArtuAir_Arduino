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
DS3231 rtc(I2C_DS3231_ADD);
SSD1306 oled;

void setupGPIO();
void setupUART();
void setupOLED();

void setup()
{
  setupUART();
  setupGPIO();
  setupOLED();
  bme680.begin();
  oled.printScreen(SSD1306::Screens::screen_welcome);
}

void loop()
{
  // Set humidity oversampling to x16
  bme680.i2c_writeByte(0x72, 0b1110000);
  // Set temperature and pressure oversampling to x16
  bme680.i2c_writeByte(0x74, 0b00111111);

  // Read temperature
  uint32_t rawt = bme680.readRawTemperature();
  double calct = bme680.calculateTemperature(rawt);

  // Read humidity
  uint32_t rawh = bme680.readRawHumidity();
  double calch = bme680.calculateHumidity(rawh);

  // Read pressure
  uint32_t rawp = bme680.readRawPressure();
  double calcp = bme680.calculatePressure(rawp);

  // Print readings
  //Serial.print("Temperature [RAW/CALC]: ");
  Serial.println(calct);
  /*Serial.print("/");
  Serial.print(calct);
  Serial.println();
  Serial.print("Humidity [RAW]: ");
  Serial.print(rawh);
  Serial.println();
  Serial.print("Pressure [RAW]: ");
  Serial.print(rawp);
  Serial.println();
  Serial.println();*/

  delay(5);
}

void setupGPIO()
{
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
}

void setupUART()
{
  Serial.begin(BAUDRATE_SERIAL);
  Serial1.begin(BAUDRATE_SERIALBT);
}

void setupOLED()
{
  // Begin OLED, generating 3.3v internally
  if (!oled.begin(SSD1306_SWITCHCAPVCC, I2C_OLED_ADD))
  {
    // Fault code = continuous red led (example)
    digitalWrite(PIN_LED_RED, HIGH);
    // Loop infinitely
    while (1)
    {
    }
  }
}
