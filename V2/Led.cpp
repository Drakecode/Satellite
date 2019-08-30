#include <Arduino.h>
#include <EEPROM.h>

#include "Satellite.h"
#include "Objet.h"
#include "Led.h"
#include "Messaging.h"

Led::Led() : Objet(LED)
{
	this->number = 0;
}

void Led::begin(uint8_t inPin, uint8_t inNumber)
{
	this->pin = inPin;
	this->number = inNumber;
	dimmer.begin(this->pin, HIGH);

	// Clignotement demo test des leds
	digitalWrite(this->pin, HIGH);
	delay(250);
	digitalWrite(this->pin, LOW);
}

void Led::loop(Satellite *inpSat)
{
	if (inpSat->modeConfig && inpSat->Bus.ConfigMessage.IsConfig() && inpSat->Bus.ConfigMessage.LedToConfig())
	{
		uint8_t number = inpSat->Bus.ConfigMessage.NumLedToConfig();
		if (number == this->number)	// Cette led est bien celle a regler...
			switch (inpSat->Bus.ConfigMessage.LedConfigType())
			{
			case LED_CONFIG_TYPE::Maximum:
				this->dimmer.setMax(inpSat->Bus.ConfigMessage.ConfigByteValue());
				this->dimmer.update();
				this->dimmer.stopBlink();
				this->dimmer.off();
				this->dimmer.on();
				break;

			case LED_CONFIG_TYPE::BrighteningTime:
				this->dimmer.setBrighteningTime(inpSat->Bus.ConfigMessage.ConfigIntValue());
				this->dimmer.stopBlink();
				this->dimmer.off();
				this->dimmer.on();
				break;

			case LED_CONFIG_TYPE::FadingTime:
				this->dimmer.setFadingTime(inpSat->Bus.ConfigMessage.ConfigIntValue());
				this->dimmer.stopBlink();
				this->dimmer.on();
				this->dimmer.off();
				break;

			case LED_CONFIG_TYPE::OnTime:
				this->dimmer.setOnTime(inpSat->Bus.ConfigMessage.ConfigIntValue());
				this->dimmer.startBlink();
				break;

			case LED_CONFIG_TYPE::Period:
				this->dimmer.setPeriod(inpSat->Bus.ConfigMessage.ConfigIntValue());
				this->dimmer.startBlink();
				break;
			}
		return;
	}

	uint8_t inNewState = inpSat->Bus.MessageIn.ledState(this->number);
	switch (inNewState) {
		case LED_BLINK:	this->dimmer.startBlink(); break;
		case LED_ON:	this->dimmer.on(); break;
		case LED_OFF:	this->dimmer.off(); break;
	}
}

#ifdef DEBUG_MODE
void Led::printObjet()
{
	Serial.print(F("Led "));
	Objet::printObjet();
	Serial.print(F("/ dimmer: "));
	Serial.print(this->dimmer.maximum());
	Serial.print(F("/ fadingTime: "));
	Serial.print(this->dimmer.fadingTime());
	Serial.print(F("/ brighteningTime: "));
	Serial.print(this->dimmer.brighteningTime());
	Serial.print(F("/ onTime: "));
	Serial.print(this->dimmer.onTime());
	Serial.print(F("/ period: "));
	Serial.print(this->dimmer.period());
}
#endif

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
  Serial.print(F("lMax "));Serial.print(valeur8); // 255
	addr += sizeof(uint8_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setFadingTime(valeur16);
  Serial.print(F(" lFad "));Serial.print(valeur16); // 250
	addr += sizeof(uint16_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setBrighteningTime(valeur16);
  Serial.print(F(" lBri "));Serial.print(valeur16); // 250
	addr += sizeof(uint16_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setOnTime(valeur16);
  Serial.print(F(" lTim "));Serial.print(valeur16); // 200
	addr += sizeof(uint16_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setPeriod(valeur16);
  Serial.print(F(" lPer "));Serial.println(valeur16); // 900
	addr += sizeof(uint16_t);

	return addr;
}


