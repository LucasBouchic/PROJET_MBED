#include "HTU21DF.h"
#include "mbed.h" // Inclure mbed pour `ThisThread::sleep_for`

HTU21DF::HTU21DF(I2C &i2c) : SensorI2C(i2c, 0x40 << 1) {}

float HTU21DF::readTemperature() {
    char cmd[1] = {0xF3};
    char data[2];
    i2c.write(address, cmd, 1);
    ThisThread::sleep_for(50ms);
    i2c.read(address, data, 2);
    uint16_t raw_temp = (data[0] << 8) | data[1];
    return -46.85 + (175.72 * raw_temp / 65536.0);
}

float HTU21DF::readHumidity() {
    char cmd[1] = {0xF5};
    char data[2];
    i2c.write(address, cmd, 1);
    ThisThread::sleep_for(50ms);
    i2c.read(address, data, 2);
    uint16_t raw_hum = (data[0] << 8) | data[1];
    return -6.0 + (125.0 * raw_hum / 65536.0);
}
