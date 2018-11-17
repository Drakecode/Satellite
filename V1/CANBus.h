//-------------------------------------------------------------------
#ifndef __configMessage_H__
#define __configMessage_H__
//-------------------------------------------------------------------

// gestion du bus (Can)

#include "Arduino.h"

#include "mcp_can.h"    // MCP2515 library
#include "mcp_can_dfs.h"

const uint8_t CANInt = 2;               // interrupt from CAN on pin 2 (int 0)
const uint8_t canCS = 10;               // 328P chip select for CAN

const int baudrate = CAN_500KBPS;       // can throughput 500 Kb/s  
const int RETRY_CONNECTION = 10;        // retry connection

class CANBus
{
public:
	unsigned long CanRxId;          // commandes ou reglages
	unsigned char RxLen = 0;        // length of data, 3 octets par message de commande
	unsigned char RxBuf[8];         // buffer of data : byte1 : code, byte2 : numero, byte3: info
  byte ext;
  
	unsigned long CanTxId;
	unsigned char TxLen = 0;        // length of data, 1 octet de donnee
	unsigned char TxBuf[8];         // buffer of data : status

public:
	void begin(uint8_t id); // init CAN

  // envoi message détecteurs
  
	void messageTx();

	// test et lecture message sur bus (CAN)
  
	byte messageRx();
};
#endif

