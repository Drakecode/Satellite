/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Classe pour les messages CAN
 */

#ifndef __CAN_MESSAGE_H__
#define __CAN_MESSAGE_H__

#include <stdint.h>

enum AIGUILLE_CONFIG_TYPE
{
	Min = B000,
	Max = B001,
	Speed = B010
};

enum LED_CONFIG_TYPE
{
	Maximum = B000,
	FadingTime = B001,
	BrighteningTime = B010,
	OnTime = B011,
	Period = B100
};

/*
 * Message CAN de commande venant du gestionnaire
 */
class CommandCANMessage
{
private:
  uint8_t mData[3];
  uint8_t dData[3];

public:
  CommandCANMessage();
  uint8_t ledState(const uint8_t inLedNumber);
  bool pointState();
  void receive(uint8_t *inData);

};

class StatusCANMessage
{
private:
  uint8_t dData[3];

public:
  StatusCANMessage();
  void setDetection(uint8_t *inData, const uint8_t inDetectNumber, bool inDetectState);
};

class ConfigCANMessage
{
private:
  uint8_t cData[8];

public:
  ConfigCANMessage();
  void receive(uint8_t *inData);

  bool IsConfig() { return  ((cData[0] & 0x80) == 0); }
  bool IsPermanentConfig() { return ((cData[0] & 0x80) != 0); }
  uint8_t LedToConfig() { return ((cData[0] & 0x01) == 1); }      // 255 si pas une Led...
  uint8_t AiguilleToConfig() { return ((cData[0] & 0x01) == 0); }  // 255 si pas une aiguille
  uint8_t NumAiguilleToConfig() { return (cData[1] >> 3); }
  uint8_t NumLedToConfig() { return (cData[1] >> 3); }
  uint8_t DetecteurToConfig() { return 255; } // 255 si pas un detecteur
  AIGUILLE_CONFIG_TYPE AiguilleConfigType() { return ((AIGUILLE_CONFIG_TYPE) (cData[1] & 0x07)); }
  LED_CONFIG_TYPE LedConfigType() { return ((LED_CONFIG_TYPE) (cData[1] & 0x07)); }
  uint8_t ConfigByteValue() { return (cData[2]); }
  uint16_t ConfigIntValue() { return ((uint16_t) (cData[2]<<8 | cData[3])); }
  float ConfigFloatValue() { 
    union sVitType {float f; unsigned char b[sizeof(float)];} ;
    sVitType sVit;
    sVit.b[3] = cData[2];
    sVit.b[2] = cData[3];
    sVit.b[1] = cData[4];
    sVit.b[0] = cData[5];
    return (sVit.f) ; 
    }

};

#endif /* __CAN_MESSAGE_H__ */

