//-------------------------------------------------------------------
#ifndef __aiguille_H__
#define __aiguille_H__
//-------------------------------------------------------------------

#include "SlowMotionServo.h"

/** Cette classe représente une aiguille. Une aiguille possède deux positions : droite et déviée. Elle est pilotée par une broche unique.
*/
class Aiguille : public Objet
{
	// Configuration

	// Gestion locale
	SMSSmooth servoMoteur;

	bool estDroit;
	
public:
	Aiguille();

	void begin(uint8_t inPin, uint8_t inNumber);
	void loop(Satellite *inpSat);
	static void loopPrioritaire() { SlowMotionServo::update(); }
	void setButee(bool inSens);

	uint8_t GetEEPROMSize()	{ return Objet::GetEEPROMSize() + (2 * sizeof(unsigned int)) + sizeof(float); }
	uint8_t EEPROM_chargement(int inAddr);
	uint8_t EEPROM_sauvegarde(int inAddr);
};
#endif

