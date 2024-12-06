#include "AS6212.h"
#include "mbed.h" // Inclure les bibliothèques nécessaires

AS6212::AS6212(I2C &i2c) : SensorI2C(i2c, 0x48 << 1) {}

float AS6212::readTemperature() {
    char cmd[1] = {0x00};
    char data[2];
    i2c.write(address, cmd, 1);
    ThisThread::sleep_for(50ms);
    i2c.read(address, data, 2);
    uint16_t raw_temp = (data[0] << 8) | data[1];
    return raw_temp / 128.0;
}