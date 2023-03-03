/**
 * @file bme680.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef BME680_H
#define BME680_H

#include <Arduino.h>
#include <Wire.h>

class BME680
{
public:
    /**
     * @brief BME680 register addresses
     *
     */
    enum class registerAddresses
    {
        ADD_STATUS = 0x73,
        ADD_RESET = 0xE0,
        ADD_ID = 0xD0,
        ADD_CONFIG = 0x75,
        ADD_CTRL_MEAS = 0x74,
        ADD_CTRL_HUM = 0x72,
        ADD_CTRL_GAS_1 = 0x71,
        ADD_CTRL_GAS_0 = 0x70,
        ADD_GAS_WAIT_X_FIRST = 0x6D,
        ADD_GAS_WAIT_X_LAST = 0x64,
        ADD_RES_HEAT_X_FIRST = 0x63,
        ADD_RES_HEAT_X_LAST = 0x5A,
        ADD_IDAC_HEAT_X_FIRST = 0x59,
        ADD_IDAC_HEAT_X_LAST = 0x50,
        ADD_GAS_R_LSB = 0x2B,
        ADD_GAS_R_MSB = 0x2A,
        ADD_HUM_LSB = 0x26,
        ADD_HUM_MSB = 0x25,
        ADD_TEMP_XLSB = 0x24,
        ADD_TEMP_LSB = 0x23,
        ADD_TEMP_MSB = 0x22,
        ADD_PRESS_XLSB = 0x21,
        ADD_PRESS_LSB = 0x20,
        ADD_PRESS_MSB = 0x1F,
        ADD_EAS_STATUS_0 = 0x1D
    };

    /**
     * @brief osrs_x settings
     * Oversampling multipliers for temperature, humidity, pressure
     *
     */
    enum class OversamplingMultipliers
    {
        osrs_skip = 0,
        osrs_x1 = 1,
        osrs_x2 = 2,
        osrs_x4 = 3,
        orsrs_x8 = 4,
        orsrs_x16 = 5
    };

    /**
     * @brief filter settings
     * IIR filtering options
     *
     */
    enum class FilterCoefficients
    {
        filter_0 = 0,
        filter_1 = 1,
        filter_3 = 2,
        filter_7 = 3,
        filter_15 = 4,
        filter_31 = 5,
        filter_63 = 6,
        filter_127 = 7
    };

    /**
     * @brief Heater wait time multipliers
     *
     */
    enum class HeaterTimeMultipliers
    {
        time_x1 = 0,
        time_x4 = 1,
        time_x16 = 2,
        time_x64 = 3
    };

    /**
     * @brief Structure containing the data read by the sensor
     *
     */
    typedef struct
    {
        // Temperature
        float temperature;

        // Pressure
        float pressure;

        // Humidity
        float humidity;

        // Gas data
        float gasResistance;
        float gasResistanceRange;

    } BMEData;

    typedef struct
    {
        // New data is available if true
        bool new_data;

        // Gas is being measured if true
        bool gas_measuring;

        // A conversion is running if true
        bool measuring;

        // The current heater set point
        uint8_t gas_measurement_index;

        // The gas is being read from the sensor (not from a randomized register) if true
        bool gas_valid;

        // Heater is stable if true
        bool heater_stability;

    } BMEStatus;

    /**
     * @brief Structure containing heater set points
     *
     */
    typedef struct
    {
        // Heater current
        uint8_t current;

        // Heater resistance
        uint8_t resistance;

        // Heater wait time before measurements, in milliseconds
        // Value boundaries: 0 to 63
        uint8_t gas_wait;

        // Heater wait time multiplication factor
        // Value boundaries: 0 to 3
        uint8_t gas_wait_multiplier;
    } BMESetPointConfig;

    /**
     * @brief Structure containing the configuration of the sensor
     *
     */
    typedef struct
    {
        // Temperature oversampling
        // Value boundaries: 0 to 5
        uint8_t osrs_t;

        // Pressure oversampling
        // Value boundaries: 0 to 5
        uint8_t osrs_p;

        // Humidity oversampling
        // Value boundaries: 0 to 5
        uint8_t osrs_h;

        // Filter coefficient
        // Value boundaries: 0 to 7
        uint8_t filter;

        // Enable gas measurements
        bool run_gas;

        // Select heater set point from BMESetPointConfig
        // Value boundaries: 0 to 9
        BMESetPointConfig set_point_cfg[10];
        uint8_t set_point;

    } BMEConfig;
private:
uint8_t i2cAdd;
BMEConfig config;
public:
    /**
     * @brief Constructs a new BME680 object
     *
     * @param i2cAddress: The I2C address of the BME680 sensor (usually 0x76 or 0x77)
     */
    BME680(uint8_t i2cAddress);
    /**
     * @brief Sets a custom configuration
     * 
     * @param cfg: The custom configuration
     */
    void setConfig(BMEConfig* cfg);
    /**
     * @brief Sets the default configuration
     * 
     */
    void setDefaultConfig();
    /**
     * @brief Writes the configuration to EEPROM
     * 
     */
    void writeConfig(uint16_t addressOffset = 0);
    /**
     * @brief Reads the configuration from EEPROM
     * 
     */
    void readConfig(uint16_t addressOffset = 0);
    /**
     * @brief Starts conversion of read data
     * 
     */
    void startConversion();
    /**
     * @brief Reads data from the sensor
     * 
     * @param data: The sensor's data (will be written at the pointed address)
     */
    void readData(BMEData* data);
};

#endif