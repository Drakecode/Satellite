#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"

#include "Satellite.h"
#include "Aiguille.h"
#include "Detecteur.h"
#include "Led.h"

#include "CANBus.h"
#include "CANMessage.h"

const uint8_t leds_pins[] = { 3, 4, 5, 6, 7, 8, 9, A0, A1 };
const uint8_t aiguilles_pins[] = { A2 };
const uint8_t detecteurs_pins[] = { A3, A4, A5 };

const char *EEPROM_ID = { "LCDO" };

void Satellite::AddObjet(Objet *inpObjet, uint8_t inPin, uint8_t inNumber)
{
	if (inpObjet != NULL && inPin != 255)
	{
		inpObjet->begin(inPin, inNumber);
		this->objets[this->nbObjets] = inpObjet;
	}
	this->nbObjets++;
}

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

	EEPROM.update(addr++, this->nbObjets);

	EEPROM.update(addr++, leds[0].GetEEPROMSize());
	EEPROM.update(addr++, aiguilles[0].GetEEPROMSize());
	EEPROM.update(addr++, detecteurs[0].GetEEPROMSize());

	////////////////////////////////////// Partie satellite

	EEPROM.update(addr++, this->id);

	////////////////////////////////////// Partie objets

	for (int i = 0; i < this->nbObjets; i++)
		if (this->objets[i] != NULL)
			addr = this->objets[i]->EEPROM_sauvegarde(addr);
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
	if (nb != this->nbObjets)
		return false;

	// taille des objets
	eeprom_read_block(buf, (const void *)addr, 3);
	addr += 3;

	if (buf[0] != leds[0].GetEEPROMSize())	return false;
	if (buf[1] != aiguilles[0].GetEEPROMSize())	return false;
	if (buf[2] != detecteurs[0].GetEEPROMSize())	return false;

	////////////////////////////////////// Partie satellite

	this->id = EEPROM.read(addr++);

	////////////////////////////////////// Partie objets

	for (int i = 0; i < this->nbObjets; i++)
		if (this->objets[i] != NULL)
			addr = this->objets[i]->EEPROM_chargement(addr);

	return true;
}

Satellite::Satellite()
{
	this->nbObjets = 0;
	for (int i = 0; i < NB_LEDS + NB_AIGUILLES + NB_DETECTEURS; i++)
		this->objets[i] = NULL;
	this->objetCourantLoop = 0;
	this->modeConfig = false;
}

void Satellite::begin(uint8_t inId)
{
	this->id = inId;
	this->Bus.begin(this->id);

	this->nbObjets = 0;
	for (int i = 0; i < NB_LEDS; i++)
	{
		this->AddObjet(&this->leds[i], leds_pins[i], i);
		// Clignotement demo test des leds
		digitalWrite(leds_pins[i], HIGH);
		delay(250);
		digitalWrite(leds_pins[i], LOW);
	}
	for (int i = 0; i < NB_AIGUILLES; i++)	
	{
		this->AddObjet(&this->aiguilles[i], aiguilles_pins[i], i);
	}
	for (int i = 0; i < NB_DETECTEURS; i++)	
	{
		this->AddObjet(&this->detecteurs[i], detecteurs_pins[i], i);
	}

	if (this->EEPROM_chargement() == false)
		this->EEPROM_sauvegarde();
}

void Satellite::loop()
{
  byte IsMessage = this->Bus.messageRx(); // RxBuf : message de commande (1) ou configuration (2) ou rien (0)
  
	if (IsMessage == 1)
	{
		this->MessageIn.receive(this->Bus.RxBuf); // recup dans mData[] et synchronisation sur les réceptions periodiques
		this->Bus.messageTx();                    // des emissions periodiques concernant les capteurs
	} 
  if (IsMessage == 2)
  {
    this->ConfigMessage.receive(this->Bus.RxBuf); // recup dans cData[] 
    // Entre en mode config dès qu'un message de config temporaire est reçu.
    this->modeConfig = true; //(this->ConfigMessage.IsConfig()) && !this->ConfigMessage.IsPermanentConfig());
    Serial.print("cfg ");Serial.println(this->modeConfig);
  }
 

	// traite les loop prioritaires
	Aiguille::loopPrioritaire();
	Detecteur::loopPrioritaire();
	Led::loopPrioritaire();

	if (this->objets[this->objetCourantLoop] != NULL)
		this->objets[this->objetCourantLoop]->loop(this);

	// puis passe à l'objet suivant pour le prochain loop...
	this->objetCourantLoop++;

	// Si on est à la fin de la liste, on recommence !
	if (this->objetCourantLoop >= this->nbObjets)
		this->objetCourantLoop = 0;

	// Si le dernier message reçu est une config permanente, sauve l'EEPROM.
	// La sauvegarde EEPROM n'écrira pas les octets déjà à la bonne valeur,
	// donc pas de danger d'écrire pour rien.
	if (this->modeConfig && this->ConfigMessage.IsPermanentConfig())
	{
		this->EEPROM_sauvegarde();
		this->modeConfig = false;
    Serial.print("cfg ");Serial.print(this->modeConfig);Serial.println(" Save");
	}
}

