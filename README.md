# Projet : Mesure de température et humidité via LoRaWAN sur carte Zest Core FMLR-72 avec le module Zest_Sensor_P-T-RH

Ce projet implémente un système embarqué pour mesurer la température et l'humidité à l'aide des capteurs **HTU21DF** et **AS6212**. Les données sont transmises via LoRaWAN par la carte **CATIE 6tron Zest Core FMLR-72**.

---

## Objectifs

- Mesurer la **température** et l'**humidité** avec des capteurs I2C.  
- Transmettre les données via LoRaWAN sur la plateforme ThingsBoard avec des étiquettes.  

---

## Matériel Requis

1. **Carte Zest Core FMLR-72**  
2. **Zest_Sensor_P-T-RH** 
3. **Environnement mbed**

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
## Récupération, Installation et Exécution du Programme

Suivez ces étapes pour récupérer et installer le programme sur votre environnement de travail. Assurez-vous de bien suivre chaque étape dans le terminal.

### Prérequis
Avant de commencer, vous devez avoir les éléments suivants installés :
- **Git** : Pour cloner le repository.
- **Mbed CLI** : Pour gérer le projet Mbed et compiler le code.
- **GCC_ARM Toolchain** : Utilisé pour compiler le programme.
- **Carte Sixtron6** et **ZEST_CORE_FMLR-72** : Matériel nécessaire pour le projet.

### Étape 1 : Cloner le repository depuis GitHub
Commencez par cloner le dépôt Git contenant le projet en utilisant la commande suivante dans votre terminal :

``` bash
   git clone https://github.com/LucasBouchic/PROJET_MBED.git
```
Cela téléchargera tous les fichiers nécessaires pour le projet dans un répertoire local sur votre machine.

### Étape 2 : Installer les librairies nécessaires

Ensuite, naviguez dans le répertoire du projet cloné et utilisez la commande mbed os pour installer toutes les librairies et dépendances nécessaires au projet :

```bash
cd PROJET_MBED
mbed os
```
Cela téléchargera et installera automatiquement toutes les librairies requises pour le bon fonctionnement du projet.

### Étape 3 : Configurer la cible Mbed

Vous devez spécifier la cible matérielle sur laquelle vous allez exécuter le programme. Dans ce cas, la cible est ZEST_CORE_CORE_FMLR-72. Utilisez la commande suivante :

```bash
mbed target ZEST_CORE_CORE_FMLR-72
```
### Étape 4 : Configurer le toolchain
Assurez-vous que le compilateur GCC_ARM est sélectionné en tant que toolchain pour la compilation. Exécutez la commande suivante :
```bash
mbed toolchain GCC_ARM
```
Cela configure Mbed CLI pour utiliser la toolchain GCC_ARM pour la compilation du projet.

### Étape 5 : Configurer le projet
Avant de compiler le projet, vous devez configurer certains paramètres de votre projet en exécutant la commande suivante dans le répertoire racine du projet :

```bash
mbed config root .
```

Cette commande configure les paramètres du projet et prépare votre environnement de travail.

### Étape 6 : Compiler le projet
Maintenant, vous pouvez compiler le projet en utilisant la commande suivante :

```bash
mbed compile
```
Cela va compiler le code source pour générer un fichier binaire qui peut être flashé sur la carte Sixtron6.

### Étape 7 : Flasher la carte Sixtron6
Une fois la compilation terminée, il est temps de flasher le programme sur la carte Sixtron6. Utilisez la commande suivante :
```bash 
sixtron flash
```
Cela va transférer le fichier binaire généré sur votre carte Sixtron6, et le programme sera exécuté immédiatement après le flashage.


## Payload Transmis

Les données envoyées au réseau LoRaWAN suivent ce format :  
```json
{
    "temperature": 22.5,
    "humidity": 40.2,
    "as6212_temperature": 22.3
}
```
## Architecture du Projet

Voici l'architecture du projet :

![Architecture du Projet](../Architecture.png)




