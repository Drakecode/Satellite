//-------------------------------------------------------------------
#ifndef __detecteur_H__
#define __detecteur_H__
//-------------------------------------------------------------------

/** Cette classe représente une détection d zone. elle peut avoir deux états : libre ou occupée.
Il y a une seule pin utilisée pour la détection.
*/
class Detecteur : public Objet
{
	// Configuration
	unsigned long intervalle;
	unsigned long remanence;
	byte etatDetecte;

	// Gestion locale
	bool estDetecte;
	bool etatPrecedent;
	unsigned long precedentTest;
	unsigned long perteDetection;

public:
	Detecteur();

	void begin(uint8_t inPin, uint8_t inNumber);
	void loop(Satellite *inpSat);
	static void loopPrioritaire() { }

	bool EstActivee() {	return this->estDetecte; }

	uint8_t GetEEPROMSize() { return Objet::GetEEPROMSize() + (2 * sizeof(unsigned long)) + 1; }
	uint8_t EEPROM_chargement(int inAddr);
	uint8_t EEPROM_sauvegarde(int inAddr);
};
#endif

