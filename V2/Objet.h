//-------------------------------------------------------------------
#ifndef __objet_H__
#define __objet_H__
//-------------------------------------------------------------------

class Satellite;

enum OBJETTYPE : byte
{
	LED = 'L',
	AIGUILLE = 'A',
	DETECT = 'D'
};

/** Cette classe est la classe de base de tous les objets utilisants une seule broche.
*/
class Objet
{
protected:
	uint8_t pin;
	uint8_t number;
	OBJETTYPE type;

public:
	Objet(OBJETTYPE inType) { this->pNextObjet = NULL; this->type = inType; this->pin = 255; this->number = 255; }

	bool IsValid() { return this->pin != 255 && this->number != 255; }
	uint8_t GetPin() {	return this->pin;	}
	uint8_t GetNumber() { return this->number; }
	OBJETTYPE GetType() {	return this->type;	}

	virtual void begin(uint8_t inPin, uint8_t inNumber) = 0;
	virtual void loop(Satellite *inpSat) = 0;
  
	virtual uint8_t GetEEPROMSize() { return 0;	}

	virtual uint8_t EEPROM_chargement(int inAddr);
	virtual uint8_t EEPROM_sauvegarde(int inAddr);

private:
	static uint8_t NombreDObjets(byte inType);

public:
	static Objet *pPremierObjet;
	Objet *pNextObjet;

	static Objet *GetLast();
	static Objet *GetObjet(OBJETTYPE inType, uint8_t inNumber);
	static void AddObjet(Objet *inpObjet, uint8_t inPin, uint8_t inNumbe);
	static uint8_t NombreDObjets() { return NombreDObjets(255);	}
	static uint8_t NombreDObjets(OBJETTYPE inType) { return NombreDObjets((byte) inType); }
	static void printObjets();

#ifdef DEBUG_MODE
	virtual void printObjet();
#endif
};

// Assure la compatibilite avec mes fonctions EEPROM dans Visual Studio...
#ifdef VISUALSTUDIO
#define EEPROMGET(SRC, DATA, SIZE)	EEPROM.get(SRC, &DATA, SIZE)
#define EEPROMPUT(SRC, DATA, SIZE)	EEPROM.put(SRC, &DATA, SIZE)
#else
#define EEPROMGET(SRC, DATA, SIZE)	EEPROM.get(SRC, DATA)
#define EEPROMPUT(SRC, DATA, SIZE)	EEPROM.put(SRC, DATA)
#endif

#endif

