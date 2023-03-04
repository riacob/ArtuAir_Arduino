/**
 * @file bme680.cpp
 * @authors Riccardo Iacob, Elia Zambelli Franz, Samuele Mininel, Enrico Bravin
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 */
#include "bme680.h"

BME680::BME680(uint8_t i2cAddress)
{
    i2cAdd = i2cAddress;
}

void BME680::begin()
{
    Wire.begin();
    // Soft reset
    i2c_writeByte(RegisterAddresses::ADD_RESET, 255);
    // Read chip id
    i2c_readByte(RegisterAddresses::ADD_ID);
    // Read variant id
    i2c_readByte(RegisterAddresses::ADD_VARIANT_ID);
    // Get calibration data
    readCalibrationParameters();
}

void BME680::i2c_writeByte(uint8_t registerAddress, uint8_t registerData)
{
    Wire.beginTransmission(i2cAdd);
    Wire.write(registerAddress);
    Wire.write(registerData);
    i2cErrno = Wire.endTransmission(true);
}

uint8_t BME680::i2c_readByte(uint8_t registerAddress)
{
    uint8_t byte;
    Wire.beginTransmission(i2cAdd);
    Wire.write(registerAddress);
    i2cErrno = Wire.endTransmission(true);
    Wire.requestFrom(i2cAdd, 1, true);
    delayMicroseconds(i2cReadDelayMicros);
    byte = Wire.read();
    return byte;
}

void BME680::setConfig(BMEConfig *cfg)
{
    config = cfg;
}

void BME680::setDefaultConfig()
{
    config->osrs_h = OversamplingMultipliers::orsrs_x16;
    config->osrs_p = OversamplingMultipliers::orsrs_x16;
    config->osrs_t = OversamplingMultipliers::orsrs_x16;
    config->filter = FilterCoefficients::filter_127;
    config->run_gas = true;
    config->set_point = HeaterSetPoints::point_0;
}

uint8_t BME680::calculateHeaterResistance(double targetTemp, double ambientTemp)
{
    // Calculate the heater resistance based on calibration parameters and desired temperature range
    // Refer to BME680 datasheet for further details on this operation
    float var1 = (calibration->par_t1 / 16) + 49.0;
    float var2 = ((calibration->par_t2 / 32768.0) * 0.0005) + 0.00235;
    float var3 = calibration->par_t3 / 1024.0;
    float var4 = var1 * (1.0 + (var2 * targetTemp));
    float var5 = var4 + (var3 * ambientTemp);
    return (3.4 * ((var5 * (4.0 / calibration->res_heat_range)) * (1.0 / (1.0 + calibration->res_heat_val * 0.002)) - 25));
}

void BME680::writeConfig(uint16_t addressOffset)
{
    EEPROM.put(addressOffset, config);
}

void BME680::readConfig(uint16_t addressOffset)
{
    EEPROM.get(addressOffset, config);
}

void BME680::readCalibrationParameters()
{
    /*
    // Read all parameters
    uint8_t rawCalibrationData[42];
    uint8_t addr1 = 0x8A;
    uint8_t addr1_len = 23;
    uint8_t addr2 = 0xE1;
    uint8_t addr2_len = 14;
    uint8_t addr3 = 0x00;
    uint8_t addr3_len = 5;
    int i = 0;
    int j = 0;
    // Read first calibration data bank
    for (i = 0; i < addr1_len; i++)
    {
        rawCalibrationData[j] = i2c_readByte(addr1);
        addr1++;
        j++;
    }
    // Read second calibration data bank
    for (i = 0; i < addr2_len; i++)
    {
        rawCalibrationData[j] = i2c_readByte(addr2);
        addr2++;
        j++;
    }
    // Read third calibration data bank
    for (i = 0; i < addr3_len; i++)
    {
        rawCalibrationData[j] = i2c_readByte(addr3);
        addr3++;
        j++;
    }
    // Save calibration data
    // uint16_t = uint8_tMSB * 256 + uint8_tLSB
    typedef BMECalRegisterIndexes cal;
    calibration->par_t1 = rawCalibrationData[cal::IDX_T1_MSB] * 256 + rawCalibrationData[cal::IDX_T1_LSB];
    calibration->par_t2 = rawCalibrationData[cal::IDX_T2_MSB] * 256 + rawCalibrationData[cal::IDX_T2_LSB];
    calibration->par_t3 = rawCalibrationData[cal::IDX_T3];
    calibration->par_p1 = rawCalibrationData[cal::IDX_P1_MSB] * 256 + rawCalibrationData[cal::IDX_P1_LSB];
    calibration->par_p2 = rawCalibrationData[cal::IDX_P2_MSB] * 256 + rawCalibrationData[cal::IDX_P2_LSB];
    calibration->par_p3 = rawCalibrationData[cal::IDX_P3];
    calibration->par_p4 = rawCalibrationData[cal::IDX_P4_MSB] * 256 + rawCalibrationData[cal::IDX_P4_LSB];
    calibration->par_p5 = rawCalibrationData[cal::IDX_P5_MSB] * 256 + rawCalibrationData[cal::IDX_P5_LSB];
    calibration->par_p6 = rawCalibrationData[cal::IDX_P6];
    calibration->par_p7 = rawCalibrationData[cal::IDX_P7];
    calibration->par_p8 = rawCalibrationData[cal::IDX_P8_MSB] * 256 + rawCalibrationData[cal::IDX_P8_LSB];
    calibration->par_p9 = rawCalibrationData[cal::IDX_P9_MSB] * 256 + rawCalibrationData[cal::IDX_P9_LSB];
    calibration->par_p10 = rawCalibrationData[cal::IDX_P10];
    calibration->par_h1 = rawCalibrationData[cal::IDX_H1_MSB] * 256 + rawCalibrationData[cal::IDX_H1_LSB];
    calibration->par_h2 = rawCalibrationData[cal::IDX_H2_MSB] * 256 + rawCalibrationData[cal::IDX_H2_LSB];
    calibration->par_h3 = rawCalibrationData[cal::IDX_H3];
    calibration->par_h4 = rawCalibrationData[cal::IDX_H4];
    calibration->par_h5 = rawCalibrationData[cal::IDX_H5];
    calibration->par_h6 = rawCalibrationData[cal::IDX_H6];
    calibration->par_h7 = rawCalibrationData[cal::IDX_H7];
    calibration->par_gh1 = rawCalibrationData[cal::IDX_GH1];
    calibration->par_gh2 = rawCalibrationData[cal::IDX_GH2_MSB] * 256 + rawCalibrationData[cal::IDX_GH2_LSB];
    calibration->par_gh3 = rawCalibrationData[cal::IDX_GH3];
    calibration->res_heat_val = rawCalibrationData[cal::IDX_RES_HEAT_VAL];
    calibration->res_heat_range = rawCalibrationData[cal::IDX_RES_HEAT_RANGE];
    calibration->range_sw_err = rawCalibrationData[cal::IDX_RANGE_SW_ERR];
    */

    typedef BMECalibrationRegistersAddresses cal;
    calibration->par_t1 = (uint16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_T1_MSB), i2c_readByte(cal::ADD_T1_LSB));
    calibration->par_t2 = (int16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_T2_MSB), i2c_readByte(cal::ADD_T2_LSB));
    calibration->par_t3 = (int8_t)i2c_readByte(cal::ADD_T3);
    calibration->par_p1 = (uint16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_P1_MSB), i2c_readByte(cal::ADD_P1_LSB));
    calibration->par_p2 = (int16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_P2_MSB), i2c_readByte(cal::ADD_P2_LSB));
    calibration->par_p3 = (int8_t)i2c_readByte(cal::ADD_P3);
    calibration->par_p4 = (int16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_P4_MSB), i2c_readByte(cal::ADD_P4_LSB));
    calibration->par_p5 = (int16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_P5_MSB), i2c_readByte(cal::ADD_P5_LSB));
    calibration->par_p6 = (int8_t)i2c_readByte(cal::ADD_P6);
    calibration->par_p7 = (int8_t)i2c_readByte(cal::ADD_P7);
    calibration->par_p8 = (int16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_P8_MSB), i2c_readByte(cal::ADD_P8_LSB));
    calibration->par_p9 = (int16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_P9_MSB), i2c_readByte(cal::ADD_P9_LSB));
    calibration->par_p10 = (uint8_t)i2c_readByte(cal::ADD_P10);
    // w
    //calibration->par_h1 = CONCAT_BYTES(i2c_readByte(cal::ADD_H1_MSB), i2c_readByte(cal::ADD_H1_LSB));
    // w
    //calibration->par_h2 = CONCAT_BYTES(i2c_readByte(cal::ADD_H2_MSB), i2c_readByte(cal::ADD_H2_LSB));
    calibration->par_h3 = (int8_t)i2c_readByte(cal::ADD_H3);
    calibration->par_h4 = (int8_t)i2c_readByte(cal::ADD_H4);
    calibration->par_h5 = (int8_t)i2c_readByte(cal::ADD_H5);
    calibration->par_h6 = (uint8_t)i2c_readByte(cal::ADD_H6);
    calibration->par_h7 = (int8_t)i2c_readByte(cal::ADD_H7);
    calibration->par_gh1 = (int8_t)i2c_readByte(cal::ADD_GH1);
    calibration->par_gh2 = (int16_t)CONCAT_BYTES(i2c_readByte(cal::ADD_GH2_MSB), i2c_readByte(cal::ADD_GH2_LSB));
    calibration->par_gh3 = (int8_t)i2c_readByte(cal::ADD_GH3);
    calibration->res_heat_val = (int8_t)i2c_readByte(cal::ADD_RES_HEAT_VAL);
    // w
    //calibration->res_heat_range = i2c_readByte(cal::ADD_RES_HEAT_RANGE);
    calibration->range_sw_err = (uint8_t)i2c_readByte(cal::ADD_RANGE_SW_ERR);
}

double BME680::calculateTemperature(uint32_t adcValue)
{
    double var1 = (((double)adcValue / 16384.0) - ((double)calibration->par_t1 / 1024.0)) * (double)calibration->par_t2;
    double var2 = ((((double)adcValue / 131072.0) - ((double)calibration->par_t1 / 8192.0)) * (((double)adcValue / 131072.0) - ((double)calibration->par_t1 / 8192.0))) * ((double)calibration->par_t3 * 16.0);
    calibration->t_fine = var1 + var2;
    double temp_comp = calibration->t_fine / 5120.0;
    //Serial.println(calibration->par_t1);
    //Serial.println(calibration->par_t2);
    //Serial.println(calibration->par_t3);
    return temp_comp;
}

double BME680::calculateHumidity(uint32_t adcValue)
{
    int32_t var1, var2, var3, var4, var5, var6, temp_scaled, calc_hum;
    temp_scaled = (((int32_t)calibration->t_fine * 5) + 128) >> 8;
    var1 = (int32_t)(adcValue - ((int32_t)((int32_t)calibration->par_h1 * 16))) - (((temp_scaled * (int32_t)calibration->par_h3) / ((int32_t)100)) >> 1);
    var2 = ((int32_t)calibration->par_h2 * (((temp_scaled * (int32_t)calibration->par_h4) / ((int32_t)100)) + (((temp_scaled * ((temp_scaled * (int32_t)calibration->par_h5) / ((int32_t)100))) >> 6) / ((int32_t)100)) + (int32_t)(1 << 14))) >> 10;
    var3 = var1 * var2;
    var4 = (int32_t)calibration->par_h6 << 7;
    var4 = ((var4) + ((temp_scaled * (int32_t)calibration->par_h7) / ((int32_t)100))) >> 4;
    var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
    var6 = (var4 * var5) >> 1;
    calc_hum = (((var3 + var6) >> 10) * ((int32_t)1000)) >> 12;
    if (calc_hum > 100000)
    {
        calc_hum = 100000;
    }
    else if (calc_hum < 0)
    {
        calc_hum = 0;
    }
    return (uint32_t)calc_hum;
}

double BME680::calculatePressure(uint32_t adcValue)
{
    int32_t var1, var2, var3, pressure_comp;
    const int32_t pres_ovf_check = INT32_C(0x40000000);
    var1 = (((int32_t)calibration->t_fine) >> 1) - 64000;
    var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * (int32_t)calibration->par_p6) >> 2;
    var2 = var2 + ((var1 * (int32_t)calibration->par_p5) << 1);
    var2 = (var2 >> 2) + ((int32_t)calibration->par_p4 << 16);
    var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * ((int32_t)calibration->par_p3 << 5)) >> 3) + (((int32_t)calibration->par_p2 * var1) >> 1);
    var1 = var1 >> 18;
    var1 = ((32768 + var1) * (int32_t)calibration->par_p1) >> 15;
    pressure_comp = 1048576 - adcValue;
    pressure_comp = (int32_t)((pressure_comp - (var2 >> 12)) * ((uint32_t)3125));
    if (pressure_comp >= pres_ovf_check)
    {
        pressure_comp = ((pressure_comp / var1) << 1);
    }
    else
    {
        pressure_comp = ((pressure_comp << 1) / var1);
    }
    var1 = ((int32_t)calibration->par_p9 * (int32_t)(((pressure_comp >> 3) * (pressure_comp >> 3)) >> 13)) >> 12;
    var2 = ((int32_t)(pressure_comp >> 2) * (int32_t)calibration->par_p8) >> 13;
    var3 = ((int32_t)(pressure_comp >> 8) * (int32_t)(pressure_comp >> 8) * (int32_t)(pressure_comp >> 8) * (int32_t)calibration->par_p10) >> 17;
    pressure_comp = (int32_t)(pressure_comp) + ((var1 + var2 + var3 + ((int32_t)calibration->par_p7 << 7)) >> 4);
    return (uint32_t)pressure_comp;
}

uint32_t BME680::readRawTemperature()
{
    // Read temperature ADC data (24-bit)
    uint8_t temp_lsb, temp_msb, temp_xlsb;
    uint32_t temp;
    temp_xlsb = i2c_readByte(RegisterAddresses::ADD_TEMP_XLSB);
    temp_lsb = i2c_readByte(RegisterAddresses::ADD_TEMP_LSB);
    temp_msb = i2c_readByte(RegisterAddresses::ADD_TEMP_MSB);
    temp = (uint32_t)(((uint32_t)temp_msb * 4096) | ((uint32_t)temp_lsb * 16) | ((uint32_t)temp_xlsb / 16));
    return temp;
}

uint32_t BME680::readRawHumidity()
{
    // Read humidity ADC data (16-bit)
    uint8_t hum_lsb, hum_msb;
    uint32_t hum;
    hum_lsb = i2c_readByte(RegisterAddresses::ADD_HUM_LSB);
    hum_msb = i2c_readByte(RegisterAddresses::ADD_HUM_MSB);
    hum = hum_msb * 256 + hum_lsb;
    return hum;
}

uint32_t BME680::readRawPressure()
{
    // Read pressure ADC data (24-bit)
    uint8_t press_lsb, press_msb, press_xlsb;
    uint32_t press;
    press_xlsb = i2c_readByte(RegisterAddresses::ADD_TEMP_XLSB);
    press_lsb = i2c_readByte(RegisterAddresses::ADD_TEMP_LSB);
    press_msb = i2c_readByte(RegisterAddresses::ADD_TEMP_MSB);
    press = (uint32_t)(((uint32_t)press_msb * 4096) | ((uint32_t)press_lsb * 16) | ((uint32_t)press_xlsb / 16));
    return press;
}
