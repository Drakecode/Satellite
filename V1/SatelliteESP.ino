/* Programme test pour  pour les cartes satellite sous base ESP
 * Adaptation de la version Satellite V1 Locoduino
 * Cyril
 * Version 0.1 09/01/2020 
 * 
 * Cette version necessite le choix du numéro de satellite avant
 * le téléchargement de l'Arduino en décommentant la ligne
 * choisie dans la liste ci-dessous.
 * D'autres paramètres sont modifiables par configuration 
 * via CAN ou Wifi
 * 
 * Cette version contient des Serial.print pour débugger
 * notamment l'affichage des valeurs en EEPROM
 * les emssages CAN & Wifi reçus
 * l'état des aiguilles
 */

#include "Satellite.h"
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

//Satellite sat;

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

// Dans l'attente d'une page de configuration - définition des paramètres wifi
//#define IP_SAT = "192.168.4.10"
#define SSID "LCDO" + ID_SAT
#define PASSWORD "LCDO"

void setup()
{ 
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  bool res;
  res = wm.autoConnect(SSID,PASSWORD); // password protected ap

	Serial.begin(115200);
  
	//sat.begin(ID_SAT);
}

/////////////////////////////////////////////////////////////////////////////

void loop() 
{
	//sat.loop();
}

/////////////////////////////////////////////////////////////////////////////
