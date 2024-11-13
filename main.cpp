/*#include "mbed.h"
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
*/

#include "mbed.h"
#include "PinNames.h"

I2C i2c(P1_I2C_SDA, P1_I2C_SCL);

// Adresse 8 bits du capteur HTU21DF (0x40 en 7 bits)
const int addr8bit = 0x40 << 1;
const int addr8bit2= 0x48 << 1; 
const int addr8bit3= 0x70 << 1;



int main()
{
    char cmd[1];  // Tableau pour envoyer des commandes
    char data[2]; // Tableau pour stocker les données lues (2 octets)
    char data2[3]; // Tableau pour stocker les données lues (3 octets)

    while (1)
    {
        // Envoi de la commande pour lire la température (0xF3)
        cmd[0] = 0xF3; // Commande pour lire la température

        i2c.write(addr8bit, cmd, 1);

        // Attente de la lecture (temps nécessaire pour le capteur)
        ThisThread::sleep_for(50ms);

        // Lire les 2 octets de température
        i2c.read(addr8bit, data, 2);

        // Convertir les 2 octets en température brute
        uint16_t raw_temp = (data[0] << 8) | data[1];
        // Calculer la température correcte en °C
        float temp = -46.85 + (175.72 * raw_temp / 65536.0);

        // Convertir la température
        int temp_int = (int)(temp * 100);

        // Afficher la température
        printf("Temperature = %d.%02d C\n", temp_int / 100, temp_int % 100);

        // Envoi de la commande pour lire l'humidité (0xF5)
        cmd[0] = 0xF5; // Commande pour lire l'humidité
        i2c.write(addr8bit, cmd, 1);

        // Attente de la lecture de l'humidité
        ThisThread::sleep_for(50ms);

        // Lire les 2 octets d'humidité
        i2c.read(addr8bit, data, 2);

        // Convertir les 2 octets en humidité brute
        uint16_t raw_hum = (data[0] << 8) | data[1];

        // Calculer l'humidité correcte en pourcentage
        float humidity = -6.0 + (125.0 * raw_hum / 65536.0);

        // Convertir l'humidité
        int hum_int = (int)(humidity * 100);

        // Afficher l'humidité
        printf("Humidity = %d.%02d %%\n", hum_int / 100, hum_int % 100);


        cmd[0] = 0x00;
        i2c.write(addr8bit2, cmd, 1);
        ThisThread::sleep_for(50ms);
        i2c.read(addr8bit2, data, 2);
        uint16_t raw_temp_AS6212 = (data[0] << 8) | data[1];
        float temp_AS6212 = (raw_temp_AS6212 / 128.0);  // Conversion selon le datasheet de AS6212
        int temp_int_AS6212 = (int)(temp_AS6212 * 100);
        printf("AS6212 Temperature = %d.%02d C\n", temp_int_AS6212 / 100, temp_int_AS6212 % 100);





        cmd[0] = 0xF9;
        i2c.write(addr8bit3, cmd, 1);
        ThisThread::sleep_for(50ms);
        i2c.read(addr8bit3, data2, 2);
        uint32_t raw_pression = (data2[0] << 16) |(data2[1] << 8) | data2[2];
        float pressure = raw_pression /256.0 ;  
        // Afficher la pression
        int pressure_int = (int)(pressure * 100);
        printf("Pressure = %d.%02d hPa\n", pressure_int / 100, pressure_int % 100);






        


        
        
        //cmd[0] = 0x01;
        //i2c.write(addr8bit2, cmd, 1);
        //ThisThread::sleep_for(50ms);
        //i2c.read(addr8bit2, data, 2);
        //char raw_cmd = (data[0] << 8) | data[1];
        //printf("cmd test = %d,%d C\n",data[0], data[1]);

        // Attente de 2 secondes avant la prochaine lecture
        ThisThread::sleep_for(500ms);
    }
}