/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include <ostream>
#include "mbed.h"
#include "PinNames.h"

// Blinking rate in milliseconds
#define BLINKING_RATE 500ms

int main()
{
    // Initialise the digital pin LED1 as an output
//#ifdef LED1
    DigitalOut led(LED1);
//#else
  //  bool led;
//#endif

//#ifdef SW1
    DigitalInOut bt(BUTTON1);
//#else
//    bool sw;
//#endif

    while (true)
    {

        if (bt.read() == 1)
        {
        led  = !led;
        }
        // led = !led;
        // ThisThread::sleep_for(BLINKING_RATE);
        //printf("Hello World!\n");
    }
}
