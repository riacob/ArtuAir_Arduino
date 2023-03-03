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
     * @brief Possible heater set points
     *
     */
    enum class HeaterSetPoints
    {
        point_0 = 0,
        point_1 = 1,
        point_2 = 2,
        point_3 = 3,
        point_4 = 4,
        point_5 = 5,
        point_6 = 6,
        point_7 = 7,
        point_8 = 8,
        point_9 = 9,
    };

    // TODO
    /**
     * @brief Possible wait times in milliseconds between heating and reading of the gas sensor
     *
     */
    enum class GasWaitMillis
    {
        millis_0 = 0,
        millis_1 = 1,
        millis_2 = 2,
        millis_3 = 3,
        millis_4 = 4,
        millis_63 = 63
    };

    /*enum class HeatRanges
    {
        range_0 = 0,
        range_1 = 1,
        range_2 = 2,
        range_3 = 3
    };*/

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
     * @brief Calibration parameters to be read from the sensor to calculate the final heater resistance
     * 
     */
    typedef struct
    {
        //calibration parameters
        uint8_t par_g1;
        uint16_t par_g2;
        uint8_t par_g3;

        //Heater range
        uint8_t res_heat_range;

        //Heater resistance correction factor
        char res_heat_val;
    } BMEResistanceParameters;

    /**
     * @brief Structure containing heater set points
     *
     */
    typedef struct
    {
        // Heater current (optional)
        //uint8_t current;

        // Heater resistance parameters
        BMEResistanceParameters resistance;

        // Heater wait time before measurements, in milliseconds
        // Value boundaries: 0 to 63
        GasWaitMillis gas_wait;

        // Heater wait time multiplication factor
        // Value boundaries: 0 to 3
        HeaterTimeMultipliers gas_wait_multiplier;
    } BMESetPointConfig;

    /**
     * @brief Structure containing the configuration of the sensor
     *
     */
    typedef struct
    {
        // Temperature oversampling
        // Value boundaries: 0 to 5
        OversamplingMultipliers osrs_t;

        // Pressure oversampling
        // Value boundaries: 0 to 5
        OversamplingMultipliers osrs_p;

        // Humidity oversampling
        // Value boundaries: 0 to 5
        OversamplingMultipliers osrs_h;

        // Filter coefficient
        // Value boundaries: 0 to 7
        FilterCoefficients filter;

        // Enable gas measurements
        bool run_gas;

        // Target temperature of the heater
        float target_temp;

        // Select heater set point from BMESetPointConfig
        // Value boundaries: 0 to 9
        BMESetPointConfig set_point_cfg[10];
        HeaterSetPoints set_point;
    } BMEConfig;

private:
    uint8_t i2cAdd;
    BMEConfig *config;

    /**
     * @brief Calculate heater resistance based on calibration parameters and desired temperature range
     * 
     * @param rParam: The resistance calibration parameters
     * @param targetTemp: The target temperatured (depending on the desired gas)
     * @param ambientTemp: The current ambient temperature (obtained by reading it trough the sensor)
     * @return float: The calculated heater resistance 
     */
    float calculateHeaterResistance(BMEResistanceParameters *rParam, double targetTemp, double ambientTemp);

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
    void setConfig(BMEConfig *cfg);

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
    void readData(BMEData *data);
};

#endif