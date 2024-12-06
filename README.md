# Projet : Mesure de température et humidité via LoRaWAN sur carte Zest Core FMLR-72 avec le module Zest_Sensor_P-T-RH

Ce projet implémente un système embarqué pour mesurer la température et l'humidité à l'aide des capteurs **HTU21DF** et **AS6212**. Les données sont transmises via LoRaWAN par la carte **CATIE 6tron Zest Core FMLR-72**.

---

## Objectifs

- Mesurer la **température** et l'**humidité** avec des capteurs I2C.  
- Transmettre les données via LoRaWAN sur la plateforme ThingsBoard avec des étiquettes.  

---

## Matériel Requis

1. **Carte Zest Core FMLR-72**  
2. **Capteur HTU21DF** (température et humidité).  
3. **Capteur AS6212** (température).  
4. **Module LoRaWAN intégré**.  
5. **Câble USB** pour programmer la carte.  
6. Un ordinateur avec **Mbed CLI** et **Git** installés.  
7. Outil de monitoring série comme **Minicom** ou **Tera Term**.  

---

## Fonctionnalités

1. **Lecture des capteurs**  
   - Récupération des données environnementales via I2C.  
   - Conversion en valeurs exploitables pour la transmission.

2. **Transmission LoRaWAN**  
   - Envoi des mesures sous forme de payload JSON.  
   - Gestion de la connexion et des événements via `LoRaWANInterface`.

3. **Optimisation de la consommation**  
   - Mise en veille de la carte entre deux relevés.  
   - Utilisation d'interruptions pour réveiller la carte lors d'événements critiques.

4. **Debug**  
   - Messages en temps réel affichés sur un terminal série.  

---

## Payload Transmis

Les données envoyées au réseau LoRaWAN suivent ce format :  
```json
{
    "temperature": 22.5,
    "humidity": 40.2,
    "as6212_temperature": 22.3
}
