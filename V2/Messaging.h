/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Constantes liées à la messagerie, partagées entre gestionnaire
 * et satellite.
 */

#ifndef __MESSAGING_H__
#define __MESSAGING_H__

static const uint8_t NUMBER_OF_LED = 9;
static const uint32_t OUT_SATELLITE_MESSAGE_TYPE = 4;
static const uint32_t NUMBER_OF_BITS_FOR_SATELLITE_ID = 3;

static const uint8_t LED_OFF = 0x0;
static const uint8_t LED_BLINK = 0x1;
static const uint8_t LED_ON = 0x2;

#endif /* __MESSAGING_H__ */

