#include <Arduino.h>
#include <EEPROM.h>

#include "Satellite.h"

const uint8_t leds_pins[] = { 3, 4, 5, 6, 7, 8, 9, A0, A1 };
const uint8_t aiguilles_pins[] = { A2 };
const uint8_t detecteurs_pins[] = { A3, A4, A5 };

const char *EEPROM_ID = { "LCDO" };

void Satellite::EEPROM_sauvegarde()
{
	int addr = 0;

	////////////////////////////////////// Partie entete

	/* Un descripteur du contenu est sauve en premier.
	Si ce descripteur n'est pas conforme a la lecture, alors l'EEPROM sera consideree vierge.
	Ca signifie aussi que changer la forme (nombre d'objets, taille d'un objet) annule toute sauvegarde !
	*/

	eeprom_update_block(EEPROM_ID, (void *)addr, 4);
	addr += 4;

	EEPROM.update(addr++, Objet::NombreDObjets());

	EEPROM.update(addr++, sizeof(Led));
	EEPROM.update(addr++, sizeof(Aiguille));
	EEPROM.update(addr++, sizeof(Detecteur));

	////////////////////////////////////// Partie satellite

	EEPROM.update(addr++, this->id);

	////////////////////////////////////// Partie objets

	uint8_t compteur = 0;
	Objet *pCurr = Objet::pPremierObjet;
	uint8_t valeur8;

	while (pCurr != NULL)
	{
		valeur8 = pCurr->GetType();
		EEPROMPUT(addr, valeur8, sizeof(uint8_t));
		addr += sizeof(uint8_t);

		addr = pCurr->EEPROM_sauvegarde(addr);
		pCurr = pCurr->pNextObjet;
	}
}

/* Chargement de la configuration depuis l'EEPROM. La fonction retourne true si tout est bien charge. */
bool Satellite::EEPROM_chargement()
{
	int addr = 0;
	char buf[5];

	////////////////////////////////////// Partie entete

	// ID EEPROM
	eeprom_read_block(buf, (const void *)addr, 4);
	addr += 4;
	buf[4] = 0;

	for (int i = 0; i < 4; i++)
		if (buf[i] != EEPROM_ID[i])
			return false;

	// Nombre d'objets
	uint8_t nb = EEPROM.read(addr++);
	if (nb != Objet::NombreDObjets())
		return false;

	// taille des objets
	eeprom_read_block(buf, (const void *)addr, 3);
	addr += 3;

	if (buf[0] != sizeof(Led))				return false;
	if (buf[1] != sizeof(Aiguille))		return false;
	if (buf[2] != sizeof(Detecteur))	return false;

	////////////////////////////////////// Partie satellite

	this->id = EEPROM.read(addr++);

	////////////////////////////////////// Partie objets

	uint8_t valeur8;
	OBJETTYPE type;
	for (int i = 0; i < nb; i++)
	{
		EEPROMGET(addr, valeur8, sizeof(uint8_t));
		type = (OBJETTYPE)valeur8;
		addr += sizeof(uint8_t);

		Objet *pNewObjet = NULL;
		switch (type)
		{
		case LED: pNewObjet = new Led();	break;
		case AIGUILLE:	pNewObjet = new Aiguille();	break;
		case DETECT:		pNewObjet = new Detecteur();	break;
		}

		if (pNewObjet != NULL)
		{
			addr = pNewObjet->EEPROM_chargement(addr);
			Objet::AddObjet(pNewObjet, pNewObjet->GetPin(), pNewObjet->GetNumber());
		}
	}

	return true;
}

Satellite::Satellite()
{
	this->pObjetCourantLoop = NULL;
	this->modeConfig = false;
}

void Satellite::begin(uint8_t inId)
{
	this->id = inId;
	this->Bus.begin(this->id);

	if (this->EEPROM_chargement() == false)
	{
		for (int i = 0; i < NB_LEDS; i++)
		{
			Led *pObjet = new Led();
			Objet::AddObjet(pObjet, leds_pins[i], i);
		}
		for (int i = 0; i < NB_AIGUILLES; i++)
		{
			Aiguille *pObjet = new Aiguille();
			Objet::AddObjet(pObjet, aiguilles_pins[i], i);
		}
		for (int i = 0; i < NB_DETECTEURS; i++)
		{
			Detecteur *pObjet = new Detecteur();
			Objet::AddObjet(pObjet, detecteurs_pins[i], i);
		}

		this->EEPROM_sauvegarde();
	}
}

void Satellite::loop()
{
  MESSAGETYPE IsMessage = this->Bus.loop(); // RxBuf : message de commande (1) ou configuration (2) ou rien (0)
  
  if (IsMessage == CONFIG)
  {
    this->modeConfig = true;
    Serial.print(F("cfg "));Serial.println(this->modeConfig);
  }
 
	// traite les loop prioritaires
	Aiguille::loopPrioritaire();
	Detecteur::loopPrioritaire();
	Led::loopPrioritaire();

	if (this->pObjetCourantLoop != NULL)
	{
		this->pObjetCourantLoop->loop(this);

		// puis passe à l'objet suivant pour le prochain loop...
		this->pObjetCourantLoop = this->pObjetCourantLoop->pNextObjet;

		// Si on est à la fin de la liste, on recommence !
		if (this->pObjetCourantLoop == NULL)
			this->pObjetCourantLoop = Objet::pPremierObjet;
	}

	// Si le dernier message reçu est une config permanente, sauve l'EEPROM.
	// La sauvegarde EEPROM n'écrira pas les octets déjà à la bonne valeur,
	// donc pas de danger d'écrire pour rien.
	if (this->modeConfig && this->ConfigMessage.IsPermanentConfig())
	{
		this->EEPROM_sauvegarde();
		this->modeConfig = false;
    Serial.print(F("cfg "));Serial.print(this->modeConfig);Serial.println(F(" Save"));
	}
}

