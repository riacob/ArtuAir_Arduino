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
  setupGPIO();
  setupUART();
  setupOLED();
  oled.printScreen(SSD1306::Screens::screen_welcome);
}

void loop()
{
  // reset
  bme680.i2c_writeByte(0xE0, 255);

  // Quick start step 1
    // configure oversampling (ctrl_hum, osrs_h x16)
  bme680.i2c_writeByte(0x72, 0b1110000);
  // configure oversampling (crtl_meas, mode 0, osrs_t x16, osrs_p x16)
  bme680.i2c_writeByte(0x74,0b00111111);

  // Quick start step 2
  // set gas_wait_0 to 0x59 for 100ms delay
  bme680.i2c_writeByte(0x6d, 0x59);
  // read and calculate resistance
  BME680::BMECalibrationParameters param;
  bme680.readCalibrationParameters(&param);
  uint8_t resistance = bme680.calculateHeaterResistance(&param,300,25);
  // write resistance to res_heat_0
  bme680.i2c_writeByte(0x63, resistance);
  // set nb_conv to 0x0 and run_gas to 1
  bme680.i2c_writeByte(0x71,0b00001000);
  // trigger a forced measurement
  bme680.i2c_writeByte(0x74,0b01111111);

  // read temp
  uint8_t temp_lsb, temp_msb, temp_xlsb;
  temp_xlsb = bme680.i2c_readByte(0x24);
  temp_lsb = bme680.i2c_readByte(0x23);
  temp_msb = bme680.i2c_readByte(0x22);
  int16_t temp = bme680.calculateTemperature(&param, ((uint32_t) (((uint32_t) temp_msb * 4096) | ((uint32_t) temp_lsb * 16)| ((uint32_t) temp_xlsb / 16))));
  Serial.println(temp);
  delay(2000);
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
