
#pragma once
#include "SensorI2C.h"

class PressureSensor : public SensorI2C {
public:
    PressureSensor(I2C &i2c) : SensorI2C(i2c, 0x70 << 1) {}

    float readPressure() {
        char cmd[1] = {0xF9};
        char data[3];
        i2c.write(address, cmd, 1);
        ThisThread::sleep_for(50ms);
        i2c.read(address, data, 3);
        uint32_t raw_pressure = (data[0] << 16) | (data[1] << 8) | data[2];
        return raw_pressure / 256.0;
    }
};