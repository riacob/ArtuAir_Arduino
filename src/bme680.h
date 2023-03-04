/**
 * @file bme680.h
 * @authors Riccardo Iacob, Elia Zambelli Franz, Samuele Mininel, Enrico Bravin
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 */
#ifndef BME680_H
#define BME680_H

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

#define CONCAT_BYTES(msb, lsb) (((uint16_t)msb << 8) | (uint16_t)lsb)

class BME680
{
public:
    /**
     * @brief BME680 register addresses
     */
    enum RegisterAddresses
    {
        ADD_VARIANT_ID = 0xF0,
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
     * @brief Indexes of elements in the calibration parameter array
     * @deprecated
     */
    enum BMECalRegisterIndexes
    {
        // Temperature
        IDX_T1_LSB = 31,
        IDX_T1_MSB = 32,
        IDX_T2_LSB = 0,
        IDX_T2_MSB = 1,
        IDX_T3 = 2,
        // Pressure
        IDX_P1_LSB = 4,
        IDX_P1_MSB = 5,
        IDX_P2_LSB = 6,
        IDX_P2_MSB = 7,
        IDX_P3 = 8,
        IDX_P4_LSB = 10,
        IDX_P4_MSB = 11,
        IDX_P5_LSB = 12,
        IDX_P5_MSB = 13,
        IDX_P6 = 15,
        IDX_P7 = 14,
        IDX_P8_LSB = 18,
        IDX_P8_MSB = 19,
        IDX_P9_LSB = 20,
        IDX_P9_MSB = 21,
        IDX_P10 = 22,
        // Humidity
        IDX_H1_LSB = 24, // weird
        IDX_H1_MSB = 25,
        IDX_H2_LSB = 24, // weird
        IDX_H2_MSB = 23,
        IDX_H3 = 26,
        IDX_H4 = 27,
        IDX_H5 = 28,
        IDX_H6 = 29,
        IDX_H7 = 30,
        // Gas
        IDX_GH1 = 35,
        IDX_GH2_LSB = 33,
        IDX_GH2_MSB = 34,
        IDX_GH3 = 36,
        // Other
        IDX_RES_HEAT_VAL = 37,
        IDX_RES_HEAT_RANGE = 39,
        IDX_RANGE_SW_ERR = 41
    };

    /**
     * @brief Addresses 0f calibration registers
     */
    enum BMECalibrationRegistersAddresses
    {
        // Temperature
        ADD_T1_LSB = 0xE9,
        ADD_T1_MSB = 0xEA,
        ADD_T2_LSB = 0x8A,
        ADD_T2_MSB = 0x8B,
        ADD_T3 = 0x8C,
        // Pressure
        ADD_P1_LSB = 0x8E,
        ADD_P1_MSB = 0x8F,
        ADD_P2_LSB = 0x90,
        ADD_P2_MSB = 0x91,
        ADD_P3 = 0x92,
        ADD_P4_LSB = 0x94,
        ADD_P4_MSB = 0x95,
        ADD_P5_LSB = 0x96,
        ADD_P5_MSB = 0x97,
        ADD_P6 = 0x99,
        ADD_P7 = 0x98,
        ADD_P8_LSB = 0x9C,
        ADD_P8_MSB = 0x9D,
        ADD_P9_LSB = 0x9E,
        ADD_P9_MSB = 0x9F,
        ADD_P10 = 0xA0,
        // Humidity
        ADD_H1_LSB = 0xE2, // 3:0
        ADD_H1_MSB = 0xE3, 
        ADD_H2_LSB = 0xE2, // 7:4
        ADD_H2_MSB = 0xE1,
        ADD_H3 = 0xE4,
        ADD_H4 = 0xE5,
        ADD_H5 = 0xE6,
        ADD_H6 = 0xE7,
        ADD_H7 = 0xE8,
        // Other
        ADD_GH1 = 0xED,
        ADD_GH2_LSB = 0xEB,
        ADD_GH2_MSB = 0xEC,
        ADD_GH3 = 0xEE,
        ADD_RES_HEAT_VAL = 0x00,
        ADD_RES_HEAT_RANGE = 0x02, // 5:4
        ADD_RANGE_SW_ERR = 0x04
    };

    /**
     * @brief osrs_x settings
     * Oversampling multipliers for temperature, humidity, pressure
     */
    enum OversamplingMultipliers
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
     */
    enum FilterCoefficients
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
     */
    enum HeaterTimeMultipliers
    {
        time_x1 = 0,
        time_x4 = 1,
        time_x16 = 2,
        time_x64 = 3
    };

    /**
     * @brief Possible heater set points
     */
    enum HeaterSetPoints
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
     */
    enum GasWaitMillis
    {
        millis_0 = 0,
        millis_1 = 1,
        millis_2 = 2,
        millis_3 = 3,
        millis_4 = 4,
        millis_5 = 4,
        millis_6 = 6,
        millis_7 = 7,
        millis_8 = 8,
        millis_9 = 9,
        millis_10 = 10,
        millis_11 = 11,
        millis_12 = 12,
        millis_13 = 13,
        millis_14 = 14,
        millis_15 = 15,
        millis_16 = 16,
        millis_17 = 17,
        millis_18 = 18,
        millis_19 = 19,
        millis_20 = 20,
        millis_21 = 21,
        millis_22 = 22,
        millis_23 = 23,
        millis_24 = 24,
        millis_25 = 25,
        millis_26 = 26,
        millis_27 = 27,
        millis_28 = 28,
        millis_29 = 29,
        millis_30 = 30,
        millis_63 = 63
    };

    /**
     * @brief Structure containing the data read by the sensor
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
     */
    typedef struct
    {
        /*! Variable to store calibrated humidity data */
        uint16_t par_h1;
        /*! Variable to store calibrated humidity data */
        uint16_t par_h2;
        /*! Variable to store calibrated humidity data */
        int8_t par_h3;
        /*! Variable to store calibrated humidity data */
        int8_t par_h4;
        /*! Variable to store calibrated humidity data */
        int8_t par_h5;
        /*! Variable to store calibrated humidity data */
        uint8_t par_h6;
        /*! Variable to store calibrated humidity data */
        int8_t par_h7;
        /*! Variable to store calibrated gas data */
        int8_t par_gh1;
        /*! Variable to store calibrated gas data */
        int16_t par_gh2;
        /*! Variable to store calibrated gas data */
        int8_t par_gh3;
        /*! Variable to store calibrated temperature data */
        uint16_t par_t1;
        /*! Variable to store calibrated temperature data */
        int16_t par_t2;
        /*! Variable to store calibrated temperature data */
        int8_t par_t3;
        /*! Variable to store calibrated pressure data */
        uint16_t par_p1;
        /*! Variable to store calibrated pressure data */
        int16_t par_p2;
        /*! Variable to store calibrated pressure data */
        int8_t par_p3;
        /*! Variable to store calibrated pressure data */
        int16_t par_p4;
        /*! Variable to store calibrated pressure data */
        int16_t par_p5;
        /*! Variable to store calibrated pressure data */
        int8_t par_p6;
        /*! Variable to store calibrated pressure data */
        int8_t par_p7;
        /*! Variable to store calibrated pressure data */
        int16_t par_p8;
        /*! Variable to store calibrated pressure data */
        int16_t par_p9;
        /*! Variable to store calibrated pressure data */
        uint8_t par_p10;
        /*! Variable to store t_fine size */
        double t_fine;
        /*! Variable to store heater resistance range */
        uint8_t res_heat_range;
        /*! Variable to store heater resistance value */
        int8_t res_heat_val;
        /*! Variable to store error range */
        int8_t range_sw_err;
    } BMECalibrationParameters;

    /**
     * @brief Structure containing heater set points
     */
    typedef struct
    {
        // Heater current (optional)
        // uint8_t current;

        // Heater resistance parameters
        // BMECalibrationParameters resistance;

        // Heater wait time before measurements, in milliseconds
        // Value boundaries: 0 to 63
        GasWaitMillis gas_wait;

        // Heater wait time multiplication factor
        // Value boundaries: 0 to 3
        HeaterTimeMultipliers gas_wait_multiplier;
    } BMESetPointConfig;

    /**
     * @brief Structure containing the configuration of the sensor
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

    // set to private
public:
    uint8_t i2cAdd;
    uint8_t i2cErrno;
    uint8_t i2cReadDelayMicros = 10;

    BMEConfig *config;
    BMECalibrationParameters *calibration;

    /**
     * @brief Calculate heater resistance based on calibration parameters and desired temperature range
     *
     * @param targetTemp: The target temperatured (depending on the desired gas)
     * @param ambientTemp: The current ambient temperature (obtained by reading it trough the sensor)
     * @return uint8_t: The calculated heater resistance
     */
    uint8_t calculateHeaterResistance(double targetTemp, double ambientTemp);

public:
    /**
     * @brief Constructs a new BME680 object
     *
     * @param i2cAddress: The I2C address of the BME680 sensor (usually 0x76 or 0x77)
     */
    BME680(uint8_t i2cAddress);

    void begin();

    /**
     * @brief Sets a custom configuration
     *
     * @param cfg: The custom configuration
     */
    void setConfig(BMEConfig *cfg);

    /**
     * @brief Sets the default configuration
     */
    void setDefaultConfig();

    /**
     * @brief Writes the configuration to EEPROM
     */
    void writeConfig(uint16_t addressOffset = 0);

    /**
     * @brief Reads the configuration from EEPROM
     */
    void readConfig(uint16_t addressOffset = 0);

    /**
     * @brief Starts conversion of read data
     */
    void startConversion();

    /**
     * @brief Reads data from the sensor
     *
     * @param data: The sensor's data (will be written at the pointed address)
     */
    void readData(BMEData *data);

    /**
     * @brief Reads calibration parameters from BME680
     */
    void readCalibrationParameters();

    /**
     * @brief Writes a byte of data to the i2c bus
     *
     * @param registerAddress: The address of the BME680's register
     * @param registerData: The data to be written at registerAddress
     */
    void i2c_writeByte(uint8_t registerAddress, uint8_t registerData);

    /**
     * @brief Reads a byte of data from the i2c bus
     *
     * @param registerAddress: The address of the BME680's register
     * @return uint8_t: The data read from registerAddress
     */
    uint8_t i2c_readByte(uint8_t registerAddress);

    /**
     * @brief Calculates temperature from raw ADC data
     * @note This function was provided by Bosch's Sensor API
     *
     * @param adcValue: The raw ADC data
     * @return double: The temperature value in °C
     */
    double calculateTemperature(uint32_t adcValue);

    /**
     * @brief Calculates humidity from raw ADC data
     * @note This function was provided by Bosch's Sensor API
     *
     * @param adcValue: The raw ADC data
     * @return uint32_t: The relative humidity value in %
     */
    double calculateHumidity(uint32_t adcValue);

    /**
     * @brief Calculates pressure from raw ADC data
     * @note This function was provided by Bosch's Sensor API
     *
     * @param adcValue: The raw ADC data
     * @return uint32_t: The pressure value in Pascal
     */
    double calculatePressure(uint32_t adcValue);

    /**
     * @brief Reads raw ADC temperature data
     *
     * @return uint32_t: The raw ADC temperature data
     */
    uint32_t readRawTemperature();

    /**
     * @brief Reads raw ADC humidity data
     *
     * @return uint32_t: The raw ADC humidity data
     */
    uint32_t readRawHumidity();

    /**
     * @brief Reads raw ADC pressure data
     *
     * @return uint32_t: The raw ADC pressure data
     */
    uint32_t readRawPressure();
};

#endif