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

float BME680::calculateHeaterResistance(BMEResistanceParameters *rParam, double targetTemp, double ambientTemp)
{
    // Calculate the heater resistance based on calibration parameters and desired temperature range
    // Refer to BME680 datasheet for further details on this operation
    float var1 = (rParam->par_g1 / 16) + 49.0;
    float var2 = ((rParam->par_g2 / 32768.0) * 0.0005) + 0.00235;
    float var3 = rParam->par_g3 / 1024.0;
    float var4 = var1 * (1.0 + (var2 * targetTemp));
    float var5 = var4 + (var3 * ambientTemp);
    return (3.4 * ((var5 * (4.0 / rParam->res_heat_range)) * (1.0 / (1.0 + rParam->res_heat_val * 0.002)) - 25));
}
