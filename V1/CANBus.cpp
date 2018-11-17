// gestion du bus (Can)

/////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
//#include <SPI.h>      //  SPI library
#include "CANBus.h"
#include "CANMessage.h"

// CAN parameters

volatile bool FlagReceive = false;      // can interrupt flag
MCP_CAN can(canCS);                     // CAN instance

// message CAN information

#ifndef VISUALSTUDIO

// CAN interrupt routine

void MCP2515_ISR() {FlagReceive = true;}

// initialisation du bus
void CANBus::begin(uint8_t id) // init CAN
{
	uint8_t canFilter = 0x20 + id;                              // messages recus = 0x20 + Id
  uint32_t extcanFilter = 0x1FFFFF00 + canFilter;
	int repeat = RETRY_CONNECTION;                              // try to open the CAN

  can.start();
  
	/*
	* set mask & filters
	* frameId = 0x20 + mSatelliteId = (0x20, 21, 22, 23, 24, 25, 26, 27}
	*/
	can.init_Mask(0, 0, 0x3FF);               // mode standard (0)
  can.init_Filt(0, 0, canFilter);           // Reception possible : Id 0x20 (hex) + mSatelliteId
  can.init_Filt(1, 0, canFilter);             

	can.init_Mask(1, 1, 0x1FFFFFFF);          // mode étendu (1)
	can.init_Filt(2, 1, extcanFilter);        // Reception possible : Id 0x1FFFFF20 (hex) + mSatelliteId (format étendu)
	can.init_Filt(3, 1, extcanFilter);        // Reception possible : Id 0x1FFFFF20 (hex) + mSatelliteId (format étendu)
	can.init_Filt(4, 0, canFilter);           // Reception possible : Id 0x20 (hex) + mSatelliteId
	can.init_Filt(5, 0, canFilter);           // Reception possible : Id 0x20 (hex) + mSatelliteId

  while (repeat > 0)
  {
    if (CAN_OK == can.begin(baudrate)) { break; }
    else { repeat--; }
    delay(500);
  }
  pinMode(CANInt, INPUT);                                       // CAN int pin
  attachInterrupt(digitalPinToInterrupt(CANInt), MCP2515_ISR, FALLING); // CAN interrupt
  
	CanTxId = 0x10 + id;                           // messages emis = 0x10 + Id
  if (repeat > 0)
  {
	  Serial.print("CAN OK avec id 0x"); Serial.println(CanTxId, HEX);
  } else {
    Serial.print("CAN NOK "); Serial.println(id, HEX);
  }

  /*
   * ajouter une séquence de conversation avec le gestionnaire pour prendre en compte
   * l'existence du satellite et pour configurer le numéro de satellite
   */
}

// envoi de message sur le bus CAN : 1 octet
void CANBus::messageTx()
{
  can.sendMsgBuf(CanTxId, 0, 1, TxBuf);
}

// test et lecture message sur bus CAN
// result = 0 : pas de message
// result = 1 : message de commande
// result = 2 : message de configuration

byte CANBus::messageRx()
{
  int ret = 0;
  if (FlagReceive) 
  { 
    // on est obligé de lire tous les messages reçu sinon l'IRQ n'arrivera plus
    while (CAN_MSGAVAIL == can.checkReceive()) 
    {
      can.readMsgBufID(&CanRxId, &RxLen, RxBuf);
      Serial.print("Rid: ");Serial.print(CanRxId, HEX);
      for (int lg = 0; lg < RxLen; lg++)
      {
        Serial.print(" 0x");Serial.print(RxBuf[lg], HEX);
      }
      if (can.isExtendedFrame()) 
      {
        Serial.println(" ext");
        ret = 2;
      } 
      else 
      {
        Serial.println(" nor ");
        ret = 1;
      }
    }
    FlagReceive = false;   // ready for next IRQ
  }
  return (ret); 
} 

/////////////////////////////////////////////////////////////////////////////
#else
void CANBus::begin(uint8_t id)
{
}

// envoi de message sur le bus (CAN)
void CANBus::messageTx()
{
}

// test et lecture message sur bus (CAN) = non utilisé => voir loop
bool CANBus::messageRx()
{
	return false;
}
#endif

