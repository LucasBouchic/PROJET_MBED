#include "mbed.h"
#include "PinNames.h"

#define INITIAL_BLINK_RATE 100ms  // Fréquence de clignotement initiale
#define MIN_COUNT 1                // Valeur minimale du compteur
#define MAX_COUNT 10               // Valeur maximale du compteur

DigitalOut led(LED1);              // Déclaration classique de la LED en sortie
InterruptIn bt(BUTTON1);           // Le bouton est déclaré sous forme d'interruption
Ticker led_freq;                   // Un objet Ticker pour le clignotement de la LED

int compteur = 0;                  // Compteur pour suivre le temps de clignotement
int max_compteur = MAX_COUNT;      // Valeur maximale du compteur (commence haut pour un clignotement lent)

// Fonction pour inverser l'état de la LED et gérer le clignotement
void flip()
{
    compteur++; // Incrémente le compteur

    // Gérer l'état de la LED en fonction du compteur
    if (compteur <= max_compteur) {
        led = 1; // LED allumée
    } else {
        led = 0; // LED éteinte
    }

    // Réinitialise le compteur si atteint max_compteur
    if (compteur >= max_compteur * 2) { // *2 car nous avons deux états (on et off)
        compteur = 0; // Réinitialise le compteur
    }
}

// Fonction pour changer la valeur maximale du compteur
void change_max_count()
{
    max_compteur--; // Décrémente la valeur maximale du compteur
    if (max_compteur < MIN_COUNT) {
        max_compteur = MAX_COUNT; // Réinitialise si inférieur à la valeur minimale définie
    }
}

int main()
{
    led_freq.attach(&flip, INITIAL_BLINK_RATE); // Démarre le clignotement avec la fréquence initiale

    // Attache l'interruption pour changer la valeur maximale du compteur à chaque pression du bouton
    bt.rise(&change_max_count); // Appel à change_max_count sur front montant du bouton

    while (true)
    {
        ThisThread::sleep_for(1s); // Facultatif : Pause pour économiser des ressources
    }
}
