#pragma once

#include "SensorI2C.h"
#include <stdio.h>

class AS6212 : public SensorI2C {
public:
    AS6212(I2C &i2c) : SensorI2C(i2c, 0x48 << 1) {}

    float readTemperature() {
        char cmd[1] = {0x00};
        char data[2];
        i2c.write(address, cmd, 1);
        ThisThread::sleep_for(50ms);
        i2c.read(address, data, 2);
        uint16_t raw_temp = (data[0] << 8) | data[1];
        return raw_temp / 128.0;
    }
};