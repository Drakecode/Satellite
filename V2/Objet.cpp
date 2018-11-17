#include <Arduino.h>
#include <EEPROM.h>

#include "Satellite.h"

Objet *Objet::pPremierObjet = NULL;

void Objet::AddObjet(Objet *inpObjet, uint8_t inPin, uint8_t inNumber)
{
	if (inpObjet != NULL && inPin != 255)
	{
		if (pPremierObjet == NULL)
		{
			pPremierObjet = inpObjet;
		}
		else
		{
			Objet *pLast = GetLast();
			pLast->pNextObjet = inpObjet;
			inpObjet->pNextObjet = NULL;
		}

		inpObjet->begin(inPin, inNumber);
	}
}

Objet *Objet::GetLast()
{
	Objet *pCurr = pPremierObjet;

	if (pCurr == NULL)
		return NULL;

	while (pCurr != NULL)
	{
		if (pCurr->pNextObjet == NULL)
			return pCurr;
		pCurr = pCurr->pNextObjet;
	}

	return NULL;
}

Objet *Objet::GetObjet(OBJETTYPE inType, uint8_t inNumber)
{
	Objet *pCurr = pPremierObjet;

	if (pCurr == NULL)
		return NULL;

	while (pCurr != NULL)
	{
		if (pCurr->type == inType && pCurr->number == inNumber)
			return pCurr;
		pCurr = pCurr->pNextObjet;
	}

	return NULL;
}

uint8_t Objet::NombreDObjets(byte inType)
{
	uint8_t compteur = 0;
	Objet *pCurr = pPremierObjet;

	if (pCurr == NULL)
		return 0;

	while (pCurr != NULL)
	{
		if (inType == 255 || (byte)pCurr->type == inType)
			compteur++;
		if (pCurr->pNextObjet == NULL)
			return compteur;
		pCurr = pCurr->pNextObjet;
	}

	return 0;
}

#ifdef DEBUG_MODE
void Objet::printObjet()
{
	Serial.print(F(" "));
	Serial.print(this->number);
	Serial.print(F(" pin: "));
	Serial.print(this->pin);
}
#endif

void Objet::printObjets()
{
#ifdef DEBUG_MODE
	Objet *pCurr = pPremierObjet;

	if (pCurr == NULL)
		return;

	while (pCurr != NULL)
	{
		pCurr->printObjet();
		Serial.println("");
		pCurr = pCurr->pNextObjet;
	}
#endif
}

uint8_t Objet::EEPROM_sauvegarde(int inAddr)
{
	int addr = inAddr;
	uint8_t valeur8;

/*	valeur8 = this->type;
	EEPROMPUT(addr, valeur8, sizeof(uint8_t));
	addr += sizeof(uint8_t);*/

	valeur8 = this->pin;
	EEPROMPUT(addr, valeur8, sizeof(uint8_t));
	addr += sizeof(uint8_t);

	valeur8 = this->number;
	EEPROMPUT(addr, valeur8, sizeof(uint8_t));
	addr += sizeof(uint8_t);

	return addr;
}

uint8_t Objet::EEPROM_chargement(int inAddr)
{
	int addr = inAddr;
	uint8_t valeur8;

/*	EEPROMGET(addr, valeur8, sizeof(uint8_t));
	this->type = (OBJETTYPE)valeur8;
	addr += sizeof(uint8_t);*/

	EEPROMGET(addr, valeur8, sizeof(uint8_t));
	this->pin = valeur8;
	addr += sizeof(uint8_t);

	EEPROMGET(addr, valeur8, sizeof(uint8_t));
	this->number = valeur8;
	addr += sizeof(uint8_t);

	return addr;
}
