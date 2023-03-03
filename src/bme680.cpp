/**
 * @file bme680.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-02-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "bme680.h"

BME680::BME680(uint8_t i2cAddress){
    i2cAdd = i2cAddress;
}

void BME680::setConfig(BMEConfig* cfg) {
    config = cfg;
}

void BME680::setDefaultConfig() {
    config->osrs_h = OversamplingMultipliers::orsrs_x16;
    config->osrs_p = OversamplingMultipliers::orsrs_x16;
    config->osrs_t = OversamplingMultipliers::orsrs_x16;
    config->filter = FilterCoefficients::filter_127;
    config->run_gas = true;
    config->set_point = HeaterSetPoints::point_0;
}

