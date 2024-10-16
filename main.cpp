#include <ostream>
#include "mbed.h"
#include "PinNames.h"

#define BLINKING_RATE 500ms

DigitalOut led(LED1);
InterruptIn bt(BUTTON1);
Timer button_timer;
Ticker led_freq;
int temps;
// Calcul du temps d'appuis
/*
void start_timer()
{
    button_timer.start();  // Démarre le chronomètre
}


void stop_timer()
{
    button_timer.stop();
    temps = button_timer.elapsed_time().count();
    button_timer.reset();
}
 */

void flip()
{
    led = !led;
}

int main()
{

    led_freq.attach(&flip, 500ms); // LED clignote à une fréquence de 1 Hz (500ms on, 500ms off)

    // bt.rise(&flip);
    // bt.rise(&start_timer);  // Appui du bouton
    // bt.fall(&stop_timer);   // Relâchement du bouton

    while (true)
    {

        // led = bt.read();
        // printf("Durée de l'appui: %d ms\n", temps / 1000);
        // led = !led;
        // ThisThread::sleep_for(BLINKING_RATE);
        //  printf("Hello World!\n");
    }
}
