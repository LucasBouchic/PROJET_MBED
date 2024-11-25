#pragma once
#include <thread>
#include <atomic>
#include <rtos.h> 
#include "mbed.h"
#include "PinNames.h"
#include <stdio.h>

class SensorI2C {
protected:
    I2C &i2c;
    int address;

public:
    SensorI2C(I2C &i2c, int address) : i2c(i2c), address(address) {}
};
