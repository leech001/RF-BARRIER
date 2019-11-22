/*
 * rf-barrier.h
 *
 *  Created on: Nov 21, 2019
 *      Author: Bulanov Konstantin
 */

#ifndef INC_RF_BARRIER_H_
#define INC_RF_BARRIER_H_

#include "main.h"

// Debug
#define RF_DEBUG

// ===== CONFIG port and pin =======
#define TXPORT	TX433_GPIO_Port
#define TXPIN	TX433_Pin

#define RXPORT	GPIOC
#define RXPIN  	GPIO_PIN_15
// ====================

typedef struct {
	uint8_t low;
	uint8_t high;
} LowHigh_t;

typedef struct {
	uint8_t	repeat;
	LowHigh_t bit1;
	LowHigh_t bit2;
} Bit_t;

typedef struct {
	uint8_t		nRepeatTransmit;
	uint16_t	PulseLength;
	uint8_t		CodeLength;
	uint8_t		Invert;
	Bit_t		Preamble;
	Bit_t		Header;
	Bit_t		Zero;
	Bit_t		One;
	Bit_t		Flt;
	Bit_t		Tail;
	uint16_t	nGuard;
} Protocol_t;

typedef struct {
	uint8_t		Invert;				// 0 if start bit from HIGH
	uint16_t	Te;
	uint16_t	TeDrv;
	uint16_t	MaxPause;
	uint16_t 	PreambleLenght;		// Length count in Te
	uint16_t	HeaderLenght;
	uint8_t		CodeLengh;
	uint16_t	TailLenght;
	uint8_t		PreambleCount;
	uint8_t		Header;
	uint8_t		BitCounter;
	uint8_t		Tail;
	uint8_t		BitArray[128];
	uint8_t 	Repeat;
	uint8_t 	BatteryLow;
	uint8_t 	Btn2;
	uint8_t 	Btn1;
	uint8_t 	Btn0;
	uint8_t 	Btn3;
	uint32_t 	SerialNum;
	uint32_t 	Encrypt;
} RF_t;

Protocol_t proto_hcs301;
Protocol_t proto_came12;
Protocol_t proto_came24;

RF_t hcs301;
RF_t came12;
RF_t nice12;

void RF_send_bin(Protocol_t* protocol, const char* sCodeWord);
void RF_send(Protocol_t* protocol, uint64_t code);
void RF_transmit(Bit_t pulses, uint16_t length, uint8_t invert);

void RX433_Int(void);
uint8_t RF_Int(RF_t* data);


#endif /* INC_RF_BARRIER_H_ */
