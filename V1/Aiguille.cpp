#include <Arduino.h>
#include <EEPROM.h>

#include "Satellite.h"
#include "Objet.h"
#include "Aiguille.h"

Aiguille::Aiguille()
{
	this->estDroit = true;
}

void Aiguille::begin(uint8_t inPin, uint8_t inNumber)
{
	this->pin = inPin;	// inutile puisque stockée dans ServoMoteur, mais puisque Objet le permet...
	this->number = inNumber;

  pinMode(this->pin, OUTPUT);
	servoMoteur.setPin(this->pin);
	servoMoteur.setMin(1200);   // faible amplitude par défaut pour éviter 
	servoMoteur.setMax(1700);   // trop de contrainte sur l'aiguille avant configuration
	servoMoteur.setSpeed(2.0);
  //servoMoteur.setReverted(true);
  servoMoteur.setInitialPosition(0.5);
  servoMoteur.goTo(this->estDroit ? 1.0 : 0.0);
}

void Aiguille::loop(Satellite *inpSat)
{
  // le test inpSat->modeConfig est ajouté dans le if et enlevé dans les case
	if (inpSat->modeConfig && inpSat->ConfigMessage.IsConfig() && inpSat->ConfigMessage.AiguilleToConfig())
	{
		uint8_t number = inpSat->ConfigMessage.NumAiguilleToConfig();
		if (number == this->number)	// Cette aiguille est bien celle à regler...
			switch (inpSat->ConfigMessage.AiguilleConfigType())
			{
			case AIGUILLE_CONFIG_TYPE::Min:
				this->servoMoteur.setupMin(inpSat->ConfigMessage.ConfigIntValue());
        this->servoMoteur.endSetup();
			  //this->servoMoteur.goTo(0.0);
				break;
			case AIGUILLE_CONFIG_TYPE::Max:
				this->servoMoteur.setupMax(inpSat->ConfigMessage.ConfigIntValue());
			  this->servoMoteur.endSetup();
			  //this->servoMoteur.goTo(1.0);
				break;
			case AIGUILLE_CONFIG_TYPE::Speed:
				this->servoMoteur.setSpeed(inpSat->ConfigMessage.ConfigFloatValue());
				this->estDroit = !this->estDroit;
				this->servoMoteur.goTo(this->estDroit ? 0.0 : 1.0);
				break;
			}
		return;
	}

	// Execution
	bool inEstDroit = inpSat->MessageIn.pointState();
	if (inEstDroit == this->estDroit)
		return; // pas de changement
  
	this->estDroit = inEstDroit;
	this->servoMoteur.goTo(this->estDroit ? 1.0 : 0.0);
}


uint8_t Aiguille::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);
	unsigned int valeurUInt;
	float valeurFloat;

	valeurUInt = this->servoMoteur.minimumPulse();
	EEPROMPUT(addr, valeurUInt, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	valeurUInt = this->servoMoteur.maximumPulse();
	EEPROMPUT(addr, valeurUInt, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	valeurFloat = this->servoMoteur.minToMaxSpeed() * 10000.f;
	EEPROMPUT(addr, valeurFloat, sizeof(float));
	addr += sizeof(float);

	return addr;
}

uint8_t Aiguille::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);
	unsigned int valeurUInt;
	float valeurFloat;

	EEPROMGET(addr, valeurUInt, sizeof(unsigned int));
	this->servoMoteur.setMin(valeurUInt);
  Serial.print("aMin ");Serial.print(valeurUInt); // 1200
	addr += sizeof(unsigned int);
	EEPROMGET(addr, valeurUInt, sizeof(unsigned int));
	this->servoMoteur.setMax(valeurUInt);
  Serial.print(" aMax ");Serial.print(valeurUInt); //1700
	addr += sizeof(unsigned int);
	EEPROMGET(addr, valeurFloat, sizeof(float));
	this->servoMoteur.setSpeed(valeurFloat / 10000.f);
  Serial.print(" aVit ");Serial.println(valeurFloat/10000.f); //2.00
	addr += sizeof(float);

	return addr;
}

