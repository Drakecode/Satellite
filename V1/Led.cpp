#include <Arduino.h>
#include <EEPROM.h>

#include "Satellite.h"
#include "Objet.h"
#include "Led.h"
#include "Messaging.h"

Led::Led()
{
	this->number = 0;
}

void Led::begin(uint8_t inPin, uint8_t inNumber)
{
	this->pin = inPin;
	this->number = inNumber;
	dimmer.begin(this->pin, HIGH);
}

void Led::loop(Satellite *inpSat)
{
  // le test inpSat->modeConfig est ajouté dans le if et enlevé dans les case
	if (inpSat->modeConfig && inpSat->ConfigMessage.IsConfig() && inpSat->ConfigMessage.LedToConfig())
	{
		uint8_t number = inpSat->ConfigMessage.NumLedToConfig();
		if (number == this->number)	// Cette led est bien celle a regler...
			switch (inpSat->ConfigMessage.LedConfigType())
			{
			case LED_CONFIG_TYPE::Maximum:
				this->dimmer.setMax(inpSat->ConfigMessage.ConfigByteValue());
          this->dimmer.update();
					this->dimmer.stopBlink();
          this->dimmer.off();
					this->dimmer.on();
				break;
			case LED_CONFIG_TYPE::BrighteningTime:
				this->dimmer.setBrighteningTime(inpSat->ConfigMessage.ConfigIntValue());
					this->dimmer.stopBlink();
					this->dimmer.off();
					this->dimmer.on();
				break;
			case LED_CONFIG_TYPE::FadingTime:
				this->dimmer.setFadingTime(inpSat->ConfigMessage.ConfigIntValue());
					this->dimmer.stopBlink();
					this->dimmer.on();
					this->dimmer.off();
				break;
			case LED_CONFIG_TYPE::OnTime:
				this->dimmer.setOnTime(inpSat->ConfigMessage.ConfigIntValue());
					this->dimmer.startBlink();
				break;
			case LED_CONFIG_TYPE::Period:
				this->dimmer.setPeriod(inpSat->ConfigMessage.ConfigIntValue());
					this->dimmer.startBlink();
				break;
			}
		return;
	}

	uint8_t inNewState = inpSat->MessageIn.ledState(this->number);
	switch (inNewState) {
		case LED_BLINK:	this->dimmer.startBlink(); break;
		case LED_ON:	this->dimmer.on(); break;
		case LED_OFF:	this->dimmer.off(); break;
	}
}
	
uint8_t Led::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);
	uint8_t valeur8;
	uint16_t valeur16;

	valeur8 = this->dimmer.maximum();
	EEPROMPUT(addr, valeur8, sizeof(uint8_t));
	addr += sizeof(uint8_t);
	valeur16 = this->dimmer.fadingTime();
	EEPROMPUT(addr, valeur16, sizeof(uint16_t));
	addr += sizeof(uint16_t);
	valeur16 = this->dimmer.brighteningTime();
	EEPROMPUT(addr, valeur16, sizeof(uint16_t));
	addr += sizeof(uint16_t);
	valeur16 = this->dimmer.onTime();
	EEPROMPUT(addr, valeur16, sizeof(uint16_t));
	addr += sizeof(uint16_t);
	valeur16 = this->dimmer.period();
	EEPROMPUT(addr, valeur16, sizeof(uint16_t));
	addr += sizeof(uint16_t);

	return addr;
}

uint8_t Led::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);
	uint8_t valeur8;
	uint16_t valeur16;

	EEPROMGET(addr, valeur8, sizeof(uint8_t));
	this->dimmer.setMax(valeur8);
  Serial.print("lMax ");Serial.print(valeur8); // 255
	addr += sizeof(uint8_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setFadingTime(valeur16);
  Serial.print(" lFad ");Serial.print(valeur16); // 250
	addr += sizeof(uint16_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setBrighteningTime(valeur16);
  Serial.print(" lBri ");Serial.print(valeur16); // 250
	addr += sizeof(uint16_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setOnTime(valeur16);
  Serial.print(" lTim ");Serial.print(valeur16); // 200
	addr += sizeof(uint16_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setPeriod(valeur16);
  Serial.print(" lPer ");Serial.println(valeur16); // 900
	addr += sizeof(uint16_t);

	return addr;
}


