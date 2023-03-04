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

uint8_t BME680::calculateHeaterResistance(BMECalibrationParameters *rParam, double targetTemp, double ambientTemp)
{
    // Calculate the heater resistance based on calibration parameters and desired temperature range
    // Refer to BME680 datasheet for further details on this operation
    float var1 = (rParam->par_t1 / 16) + 49.0;
    float var2 = ((rParam->par_t2 / 32768.0) * 0.0005) + 0.00235;
    float var3 = rParam->par_t3 / 1024.0;
    float var4 = var1 * (1.0 + (var2 * targetTemp));
    float var5 = var4 + (var3 * ambientTemp);
    return (3.4 * ((var5 * (4.0 / rParam->res_heat_range)) * (1.0 / (1.0 + rParam->res_heat_val * 0.002)) - 25));
}

void BME680::writeConfig(uint16_t addressOffset)
{
    EEPROM.put(addressOffset, config);
}

void BME680::readConfig(uint16_t addressOffset)
{
    EEPROM.get(addressOffset, config);
}

void BME680::readCalibrationParameters(BMECalibrationParameters *param)
{
    uint8_t memorydump[256];
    for(int k = 0; k < 256; k++) {
        memorydump[k] = i2c_readByte(k);
    }

    for (int m = 0; m < 256; m++) {
        Serial.print(memorydump[m]);
        Serial.print(" ");
    }
    Serial.println();

    // Read all parameters
    uint8_t rawCalibrationData[41] = {0};
    uint8_t addr1 = 0x8a;
    uint8_t addr2 = 0xe1;
    int i;
    for (i = 0; i < 25; i++)
    {
        rawCalibrationData[i] = i2c_readByte(addr1);
        addr1++;
    }
    for (i = 0; i < 16; i++)
    {
        rawCalibrationData[i] = i2c_readByte(addr2);
        addr2++;
    }

    // Print all parameters to serial
    
    for (i = 0; i < 41; i++) {
        Serial.print(rawCalibrationData[i], DEC);
        Serial.print(" ");
    }
    Serial.println();
    

    // Read temperature parameters
    /*
    uint8_t par_t1_msb, par_t1_lsb, par_t2_msb, par_t2_lsb, par_t3;
    uint16_t par_t1, par_t2;

    par_t2_lsb = i2c_readByte(0x39);
    par_t2_msb = i2c_readByte(0x39 + 1);
    par_t3 = i2c_readByte(0x39 + 2);
    par_t1_lsb = i2c_readByte(0xe1 + 10);
    par_t1_msb = i2c_readByte(0xe1 + 11);

    par_t1 = (par_t1_lsb << 8) | par_t1_msb;
    par_t2 = (par_t1_lsb << 8) | par_t1_msb;

    param->par_t1 = par_t1;
    param->par_t2 = par_t2;
    param->par_t3 = par_t3;
    */
}

int16_t BME680::calculateTemperature(BMECalibrationParameters *param, uint32_t adc_cts)
{
    // Calculate temperature based on calibration parameters
    int64_t var1;
    int64_t var2;
    int64_t var3;
    int16_t calc_temp;

    var1 = ((int32_t)adc_cts >> 3) - ((int32_t)param->par_t1 << 1);
    var2 = (var1 * (int32_t)param->par_t2) >> 11;
    var3 = ((var1 >> 1) * (var1 >> 1)) >> 12;
    var3 = ((var3) * ((int32_t)param->par_t3 << 4)) >> 14;
    param->t_fine = (int32_t)(var2 + var3);
    calc_temp = (int16_t)(((param->t_fine * 5) + 128) >> 8);

    return calc_temp;
}
