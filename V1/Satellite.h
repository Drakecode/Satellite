/* Affectation des pins 
 *  MINI / UNO / NANO
 * pin 0 = RX
 * pin 1 = TX
 * pin 2 = CANInt
 * pin 3 = Led 0
 * pin 4 = Led 1
 * pin 5 = Led 2
 * pin 6 = Led 3
 * pin 7 = Led 4
 * pin 8 = Led 5
 * pin 9 = Led 6
 * pin 10 = CANCS
 * pin 11 = MOSI
 * pin 12 = MISO
 * pin 13 = SCK
 * pin A0 = Led 7
 * pin A1 = Led 8
 * pin A2 = Aiguille 0
 * pin A3 = Balise 0
 * pin A4 = Balise 1
 * pin A5 = Zone 0
 * pin A6 (nu)
 * pin A7 (nu)
 */ 

/* Affectation des pins 
 *  ESP32 / NOdeMCU8266
 * pin 0 = 
 * ....
 * pin 13 =
 * pin A0 = 
 */


 
#ifndef _Satellite_H_
#define _Satellite_H_

#if !defined (__AVR_ATmega328P__)
#if !defined (ARDUINO_ESP8266_NODEMCU) || !defined (ESP8266)
#if !defined VISUALSTUDIO
  #error CANNOT COMPILE - ONLY WORKS WITH AN ARDUINO UNO, NANO OR MINI OR ESP32/8266
#endif
#endif
#endif

// Choix de la communication
//#define USE_CAN
#define USE_MQTT


#include <Arduino.h>

#include "Objet.h"

#include "Aiguille.h"
#include "Detecteur.h"
#include "Led.h"

#if defined(USE_CAN)
#include "CANBus.h"
#include "CANMessage.h"
#endif

#define NB_LEDS			9
#define NB_AIGUILLES	1
#define NB_DETECTEURS	3

class Satellite
{
private:
	// Configuration
	uint8_t		id;	// Identifiant du satellite

	// Gestion locale	
	Led			leds[NB_LEDS];
	Aiguille	aiguilles[NB_AIGUILLES];
	Detecteur	detecteurs[NB_DETECTEURS];

	Objet*		objets[NB_LEDS + NB_AIGUILLES + NB_DETECTEURS];	// Liste de tous les objets gérés
	uint8_t		nbObjets;
	byte		objetCourantLoop;

	void AddObjet(Objet *inpObjet, uint8_t inPin, uint8_t inNumber);
	void EEPROM_sauvegarde();
	bool EEPROM_chargement();

public:
#if defined(USE_CAN)	
	CANBus Bus;
	CommandCANMessage MessageIn;
	StatusCANMessage StatusMessage;
  ConfigCANMessage ConfigMessage;
#endif
	bool	modeConfig;

	Satellite();
	void begin(uint8_t inId);
	void loop();
};
#endif
