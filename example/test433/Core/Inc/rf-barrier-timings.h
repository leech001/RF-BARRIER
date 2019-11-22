/*
 * rf-barrier-timings.h
 *
 *  Created on: Nov 21, 2019
 *      Author: Bulanov Konstantin
 */

#ifndef INC_RF_BARRIER_TIMINGS_H_
#define INC_RF_BARRIER_TIMINGS_H_

// =============Receive========
// Keeloq
RF_t keeloq = {
		.Invert=			1,
		.Te = 				400,
		.TeDrv = 			50,
		.MaxPause = 		20000,
		.CodeLengh = 		66,
		.PreambleLenght = 	23,
		.HeaderLenght=		10
};

// Came 12
RF_t came12 = {
		.Invert=			0,
		.Te =	 			320,
		.TeDrv =	 		50,
		.MaxPause = 		20000,
		.CodeLengh = 		12,
		.PreambleLenght = 	0,
		.HeaderLenght=		1
};

// Nice 12
RF_t nice12 = {
		.Invert=			0,
		.Te =	 			700,
		.TeDrv =	 		50,
		.MaxPause = 		20000,
		.CodeLengh = 		12,
		.PreambleLenght = 	0,
		.HeaderLenght=		1
};


// ========== Transmit ===========
Protocol_t proto_keeloq ={
		.nRepeatTransmit =		1,		// Repeat code
		.PulseLength = 			400,
		.CodeLength = 			66,
		.Invert = 				1,		// Start from HIGH

		.Preamble.repeat = 		12,
		.Preamble.bit1.low = 	1,
		.Preamble.bit1.high = 	1,
		.Preamble.bit2.low = 	0,
		.Preamble.bit2.high = 	0,

		.Header.repeat =		9,		// 9 not 10 when one low pulse send from Preamble.
		.Header.bit1.low = 		1,
		.Header.bit1.high = 	0,
		.Header.bit2.low = 		0,
		.Header.bit2.high = 	0,

		.Zero.repeat = 			1,
		.Zero.bit1.low = 		1,
		.Zero.bit1.high = 		2,
		.Zero.bit2.low = 		0,
		.Zero.bit2.high = 		0,

		.One.repeat =			1,
		.One.bit1.low = 		2,
		.One.bit1.high = 		1,
		.One.bit2.low = 		0,
		.One.bit2.high = 		0,

		.Tail.repeat = 			2,		// 64 Code + 2 Tail = 66 bit (Keeloq)
		.Tail.bit1.low = 		2,
		.Tail.bit1.high = 		1,
		.Tail.bit2.low = 		0,
		.Tail.bit2.high = 		0,

		.nGuard = 				39
};

Protocol_t proto_came12 ={
		.nRepeatTransmit =		4,		// Repeat code
		.PulseLength = 			320,
		.CodeLength = 			12,
		.Invert = 				0,

		.Preamble.repeat = 		36,
		.Preamble.bit1.low = 	1,
		.Preamble.bit1.high = 	0,
		.Preamble.bit2.low = 	0,
		.Preamble.bit2.high = 	0,

		.Header.repeat =		1,
		.Header.bit1.low = 		0,
		.Header.bit1.high = 	1,
		.Header.bit2.low = 		0,
		.Header.bit2.high = 	0,

		.Zero.repeat = 			1,
		.Zero.bit1.low = 		1,
		.Zero.bit1.high = 		2,
		.Zero.bit2.low = 		0,
		.Zero.bit2.high = 		0,

		.One.repeat =			1,
		.One.bit1.low = 		2,
		.One.bit1.high = 		1,
		.One.bit2.low = 		0,
		.One.bit2.high = 		0,

		.Tail.repeat = 			0,
		.Tail.bit1.low = 		0,
		.Tail.bit1.high = 		0,
		.Tail.bit2.low = 		0,
		.Tail.bit2.high = 		0,

		.nGuard = 				0
};

Protocol_t proto_came24 ={
		.nRepeatTransmit =		4,		// Repeat code
		.PulseLength = 			320,
		.CodeLength = 			24,
		.Invert = 				0,

		.Preamble.repeat = 		36,
		.Preamble.bit1.low = 	1,
		.Preamble.bit1.high = 	0,
		.Preamble.bit2.low = 	0,
		.Preamble.bit2.high = 	0,

		.Header.repeat =		1,
		.Header.bit1.low = 		0,
		.Header.bit1.high = 	1,
		.Header.bit2.low = 		0,
		.Header.bit2.high = 	0,

		.Zero.repeat = 			1,
		.Zero.bit1.low = 		1,
		.Zero.bit1.high = 		2,
		.Zero.bit2.low = 		1,
		.Zero.bit2.high = 		2,

		.One.repeat =			1,
		.One.bit1.low = 		2,
		.One.bit1.high = 		1,
		.One.bit2.low = 		2,
		.One.bit2.high = 		1,

		.Tail.repeat = 			0,
		.Tail.bit1.low = 		0,
		.Tail.bit1.high = 		0,
		.Tail.bit2.low = 		0,
		.Tail.bit2.high = 		0,

		.nGuard = 				0
};


#endif /* INC_RF_BARRIER_TIMINGS_H_ */
