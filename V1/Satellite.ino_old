/* Programme pour les cartes satellite du Locoduinodrome
 * Thierry 04 Avril 2018 pour la conception
 * Dominique 25 Avril 2018 pour les tests
 * Copyright Locoduino 2018
 * Version 0.2 du 12 Juin 2018 
 * 
 * Cette version necessite le choix du numéro de satellite avant
 * le téléchargement de l'Arduino en décommentant la ligne
 * choisie dans la liste ci-dessous.
 * Tous les autres paramètres sont modifiables par configuration 
 * via CAN.
 * 
 * Cette version contient des Serial.print pour débugger
 * notamment l'affichage des valeurs en EEPROM
 * les emssages CAN reçus
 * l'état des aiguilles
 */

#include "Satellite.h"

Satellite sat;

/*
 * Temporairement on doit fixer l'Id des satellites a la main 
 * avant téléchardement de l'Arduino
 */

//#define ID_SAT        0 // C3 carre 4, Z0, P3, P5
//#define ID_SAT        1 // C4 carre 5, Z1, P4, P6
//#define ID_SAT        2 // C6 carre 7 et aiguille a0 et Z2
//#define ID_SAT        3 // S2 semaphore 1 et S3 semaphore RR 8, Z3, P8, P9
//#define ID_SAT        4 // S1 semaphore 0 et S4 semaphore RR 9, Z6, P7, P10
//#define ID_SAT        5 // C2 carre RR 3, Z4, P2
#define ID_SAT        6 // C5 carre 6 et aiguille a1, Z5
//#define ID_SAT        7 // C1 carre RR 2, P1


void setup()
{ 
	Serial.begin(115200);
  
	sat.begin(ID_SAT);
}

/////////////////////////////////////////////////////////////////////////////

void loop() 
{
	sat.loop();
}

/////////////////////////////////////////////////////////////////////////////
