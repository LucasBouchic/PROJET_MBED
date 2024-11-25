#include "mbed.h"
#include "PinNames.h"
#include <stdio.h>

// Déclarations globales
I2C i2c(P1_I2C_SDA, P1_I2C_SCL);
DigitalOut led(LED1);    // Déclaration de la LED
InterruptIn bt(BUTTON1); // Le bouton est déclaré sous forme d'interruption

// Adresses des capteurs
const int addr8bit = 0x40 << 1;
const int addr8bit2 = 0x48 << 1;
const int addr8bit3 = 0x70 << 1;

// Déclaration des variables pour le capteur de pression
int16_t bt1_0, bt1_1, bt2_1, bt2_0, bp1_0, bp1_1, b11_0, b11_1;

// Déclaration des fonctions
int32_t get_coefficient(char high_byte, char low_byte);
void read_coefficients();

// Fonction pour afficher un entier en binaire
void print_binary(int16_t coeff)
{
    for (int i = 15; i >= 0; --i) {
        // Affiche chaque bit de 'coeff' en commençant par le bit de poids fort
        printf("%d", (coeff >> i) & 1);
        if (i % 4 == 0) {
            printf(" "); // Ajoute un espace tous les 4 bits pour la lisibilité
        }
    }
    printf("\n");
}

// Fonction pour lire les coefficients du capteur 2SMPB-02E
void read_coefficients()
{
    char cmd[1];
    char data[2];

    data[0] = 0xA2; // 
    i2c.write(addr8bit3, cmd, 1);
    i2c.read(addr8bit3, data, 2);
    bt1_1 = get_coefficient(data[0], data[1]);

    data[0] = 0xA3; // 
    i2c.write(addr8bit3, cmd, 1);
    i2c.read(addr8bit3, data, 2);
    bt1_0 = get_coefficient(data[0], data[1]);

    data[0] = 0xA4; // 
    i2c.read(addr8bit3, data, 2);
    i2c.write(addr8bit3, cmd, 1);
    bt2_1 = get_coefficient(data[0], data[1]);

    data[0] = 0xA5; // 
    i2c.read(addr8bit3, data, 2);
    i2c.write(addr8bit3, cmd, 1);
    bt2_0 = get_coefficient(data[0], data[1]);

    data[0] = 0xA6; // 
    i2c.read(addr8bit3, data, 2);
    i2c.write(addr8bit3, cmd, 1);
    bp1_1 = get_coefficient(data[0], data[1]);

    data[0] = 0xA7; // 
    i2c.read(addr8bit3, data, 2);
    i2c.write(addr8bit3, cmd, 1);
    bp1_0 = get_coefficient(data[0], data[1]);


    // Afficher les résultats des coefficients
    printf("Coefficient b00_1: %ld\n", b00_1);
    printf("Coefficient b00_0: %ld\n", b00_0);
    printf("Coefficient b00_a0_ex: %ld\n", b00_a0_ex);
}

// Fonction pour reconstruire la valeur du coefficient à partir de deux octets
int32_t get_coefficient(char high_byte, char low_byte, char xxx)
{
    // Combiner les deux octets en une valeur 16 bits
    //uint32_t coeff = (high_byte << 8) | low_byte << ...;
    uint32_t res ;

    // Si le bit MSB est 1, cela indique un nombre négatif
    if (coeff & 0x00080000)
    {
        res = 0xFFF00000 + coeff ; // Complément à deux pour un nombre négatif
    }
    else
    {
        res = coeff ;
    }
    // Afficher le coefficient en format binaire
    printf("Coefficient in binary: ");
    print_binary(res);
    return res;
}

/*id read_pressure()
{
    char cmd[1] = {0xF7};
    char data[3];
    if (i2c.write(addr8bit3, cmd, 1) == 0 && i2c.read(addr8bit3, data, 3) == 0)
    {
        uint32_t raw_pressure = (data[0] << 16) | (data[1] << 8) | data[2];

        // Lire les données brutes de température (Dt)
        char temp_cmd[1] = {0xFA}; // Commande pour lire la température brute
        char temp_data[3];
        i2c.write(addr8bit3, temp_cmd, 1);
        i2c.read(addr8bit3, temp_data, 3);
        uint32_t raw_temp = (temp_data[0] << 16) | (temp_data[1] << 8) | temp_data[2];

        // Calcul de la température Tr en utilisant les coefficients a0, a1, a2
        float Dt = raw_temp / 256.0f;           // Conversion de la donnée brute en température
        float Tr = a0 + a1 * Dt + a2 * Dt * Dt; // Calcul de Tr

        // Convertir Tr en température en degrés Celsius
        float temperature = Tr / 256.0f;

        // Affichage des coefficients pour la vérification
        printf("Coefficients:\n");
        printf("b00 = %ld\n", b00);
        printf("bt1 = %ld\n", bt1);
        printf("bp1 = %ld\n", bp1);
        printf("b11 = %ld\n", b11);
        printf("bt2 = %ld\n", bt2);
        printf("bp2 = %ld\n", bp2);
        printf("b12 = %ld\n", b12);
        printf("b21 = %ld\n", b21);
        printf("bp3 = %ld\n", bp3);
        printf("a0 = %.ld\n", a0);
        printf("a1 = %.ld\n", a1);
        printf("a2 = %.ld\n", a2);

        // Calcul de la pression en fonction des coefficients
        float pressure = b00 + (bt1 * raw_pressure) + (bp1 * raw_pressure * raw_pressure) +
                         (b11 * raw_pressure * raw_pressure * raw_pressure) +
                         (bt2 * raw_pressure * raw_pressure) +
                         (bp2 * raw_pressure * raw_pressure * raw_pressure) +
                         (b12 * raw_pressure) +
                         (b21 * raw_pressure) +
                         (bp3 * raw_pressure * raw_pressure * raw_pressure);

        // Affichage de la pression et de la température
        // Calcul de la pression et de la température
        printf("Raw Pressure = %ld Pa\n", raw_pressure);
        printf("Calculated Temperature = %.2f C\n", temperature);
        printf("Pressure = %.2f Pa\n", last_pressure);

        last_pressure = pressure; // Mettre à jour la pression
        pressure_ready = true;    // Indiquer qu'une nouvelle donnée est prête
    }
}*/

// Fonction pour lire la température HTU21DF
float read_temperature_HTU21DF()
{
    char cmd[1] = {0xF3}; // Commande pour lire la température
    char data[2];

    // Envoi de la commande et lecture de la température
    i2c.write(addr8bit, cmd, 1);
    ThisThread::sleep_for(50ms);
    i2c.read(addr8bit, data, 2);

    // Convertir les 2 octets en température brute
    uint16_t raw_temp = (data[0] << 8) | data[1];

    // Calculer la température correcte en °C
    return -46.85 + (175.72 * raw_temp / 65536.0);
}

// Fonction pour lire l'humidité HTU21DF
float read_humidity_HTU21DF()
{
    char cmd[1] = {0xF5}; // Commande pour lire l'humidité
    char data[2];

    // Envoi de la commande et lecture de l'humidité
    i2c.write(addr8bit, cmd, 1);
    ThisThread::sleep_for(50ms);
    i2c.read(addr8bit, data, 2);

    // Convertir les 2 octets en humidité brute
    uint16_t raw_hum = (data[0] << 8) | data[1];

    // Calculer l'humidité correcte en pourcentage
    return -6.0 + (125.0 * raw_hum / 65536.0);
}

// Fonction pour lire la température AS6212
float read_temperature_AS6212()
{
    char cmd[1] = {0x00}; // Commande pour lire la température AS6212
    char data[2];

    // Envoi de la commande et lecture de la température
    i2c.write(addr8bit2, cmd, 1);
    ThisThread::sleep_for(50ms);
    i2c.read(addr8bit2, data, 2);

    // Convertir les 2 octets en température brute et calculer la température réelle
    uint16_t raw_temp = (data[0] << 8) | data[1];
    return raw_temp / 128.0; // Conversion selon le datasheet de l'AS6212
}

// Fonction pour afficher les valeurs lues
void print_sensor_data()
{
    // Affichage des données des capteurs
    float temp_HTU21DF = read_temperature_HTU21DF();
    int temp_int_HTU21DF = (int)(temp_HTU21DF * 100);
    printf("HTU21DF Temperature = %d.%02d C\n", temp_int_HTU21DF / 100, temp_int_HTU21DF % 100);

    float humidity_HTU21DF = read_humidity_HTU21DF();
    int hum_int_HTU21DF = (int)(humidity_HTU21DF * 100);
    printf("HTU21DF Humidity = %d.%02d %%\n", hum_int_HTU21DF / 100, hum_int_HTU21DF % 100);

    float temp_AS6212 = read_temperature_AS6212();
    int temp_int_AS6212 = (int)(temp_AS6212 * 100);
    printf("AS6212 Temperature = %d.%02d C\n", temp_int_AS6212 / 100, temp_int_AS6212 % 100);

}

// Fonction principale
int main()
{
    // Lire les coefficients du capteur de pression
    read_coefficients();

    // Boucle principale
    while (true)
    {
        print_sensor_data(); // Afficher les données des capteurs
        // read_pressure();              // Lire la pression
        ThisThread::sleep_for(500ms); // Attente de 500ms avant la prochaine lecture
    }
}

/*
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

/*
#include "mbed.h"
#include "PinNames.h"

#include <thread>
#include <atomic>
#include <rtos.h> // Inclus pour utiliser les threads de mbed OS

Mutex printf_mutex;

DigitalOut led(LED1);             // Déclaration de la LED
InterruptIn bt(BUTTON1);          // Le bouton est déclaré sous forme d'interruption

std::atomic<bool> ping_running(true);  // Flag pour suivre l'état de ping
std::atomic<bool> pong_running(true); // Flag pour suivre l'état de pong

// Fonction ping
void ping() {
    for (int i = 0; i < 100; ++i) {
        printf_mutex.lock();
        printf("Ping\n");
        printf_mutex.unlock();
    }
}

// Fonction pong
void pong() {
    for (int i = 0; i < 100; ++i) {
        printf_mutex.lock();
        printf("Pong\n");
        printf_mutex.unlock();
    }
}

int main() {

    // Création des threads avec une taille de pile personnalisée (par exemple 1024 octets)
    Thread ping_thread(osPriorityNormal, 1024, nullptr, nullptr);  // Taille de pile : 1024 octets
    Thread pong_thread(osPriorityNormal, 1024,nullptr,nullptr);  // Taille de pile : 1024 octets

    // Démarre les threads ping et pong
    ping_thread.start(callback(ping));
    pong_thread.start(callback(pong));

    // Boucle principale pour afficher "Alive" lorsque les threads sont encore en cours
    while (1) {
        led = !led;  // Clignote la LED
        printf("Alive\n");
        ThisThread::sleep_for(2s);  // Affiche "Alive" toutes les 2 secondes
    }

    return 0;
}
*/