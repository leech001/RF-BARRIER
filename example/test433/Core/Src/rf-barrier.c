/*
 * rf-barrier.c
 *
 *  Created on: Nov 21, 2019
 *      Author: Bulanov Konstantin
 *
 *  Contact information
 *  -------------------
 *
 * e-mail   :  leech001@gmail.com
 *
 *
 */

/*
 * |-----------------------------------------------------------------------------------------------------------------------------------------------
 * | Copyright (C) Bulanov Konstantin,2019
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |
 * | Came and Nice algorithm http://phreakerclub.com/forum/showthread.php?t=126
 * |------------------------------------------------------------------------------------------------------------------------------------------------
 */


#include <stdio.h>
#include <string.h>
#include "micros.h"
#include "rf-barrier.h"
#include "rf-barrier-timings.h"


#ifdef RF_DEBUG
#include <usbd_cdc_if.h>
char str_code[512] = {0,};
#endif


void RF_send_bin(Protocol_t* protocol, const char* sCodeWord) {
  uint64_t code = 0;
  for (const char* p = sCodeWord; *p; p++) {
	  code <<= 1ULL;
	  if (*p != '0'){
		  code |= 1ULL;
	  }
  }
  RF_send(protocol, code);
}

void RF_send(Protocol_t* protocol, uint64_t code) {
	for (int nRepeat = 0; nRepeat < protocol->nRepeatTransmit; nRepeat++){

		// Preamble
		RF_transmit(protocol->Preamble, protocol->PulseLength, protocol->Invert);

		// Header
		RF_transmit(protocol->Header, protocol->PulseLength, protocol->Invert);

		// Code
		for (int i = protocol->CodeLength-1; i >= 0; i--){
			if (code & (1ULL << i)){
				RF_transmit(protocol->One, protocol->PulseLength, protocol->Invert);
			}else{
				RF_transmit(protocol->Zero, protocol->PulseLength, protocol->Invert);
			}
		}

		// Tail
		RF_transmit(protocol->Tail, protocol->PulseLength, protocol->Invert);

		#ifdef RF_DEBUG
		char usb_str[512]={0,};
		sprintf(usb_str,"%s\r\n", str_code);
	    CDC_Transmit_FS((unsigned char *) usb_str, (uint16_t) strlen(usb_str));
	    memset(str_code, '\0', sizeof(str_code));
		#endif

		//Guard delay
		if (protocol->nGuard > 0){
			HAL_GPIO_WritePin(TXPORT, TXPIN, GPIO_PIN_RESET);
			delay_us(protocol->PulseLength * protocol->nGuard);
		}
	}
}

void RF_transmit(Bit_t pulses, uint16_t length, uint8_t invert) {

	uint8_t bits[2][2] = {{pulses.bit1.low, pulses.bit1.high},
						  {pulses.bit2.low, pulses.bit2.high}};
	uint8_t PulseOne;
	uint8_t PulseTwo;
	uint8_t LevelOne;
	uint8_t LevelTwo;

	for(int i=0; i < pulses.repeat; i++){
		for (int i_bits = 0; i_bits < 2; i_bits++){
			if(invert == 0){
				PulseOne = bits[i_bits][0];
				PulseTwo = bits[i_bits][1];
				LevelOne =  GPIO_PIN_RESET;
				LevelTwo =  GPIO_PIN_SET;
			}else{
				PulseOne = bits[i_bits][1];
				PulseTwo = bits[i_bits][0];
				LevelOne =  GPIO_PIN_SET;
				LevelTwo =  GPIO_PIN_RESET;
			}

			if (PulseOne > 0){
				HAL_GPIO_WritePin(TXPORT, TXPIN, LevelOne);
				delay_us(length * PulseOne);

				#ifdef RF_DEBUG
				for(int i = 0; i < PulseOne; i++)
					if(LevelOne == 1){
						sprintf(str_code,"%s-", str_code);
					}else{
						sprintf(str_code,"%s_", str_code);
					}
				#endif
			}
			if (PulseTwo > 0){
				HAL_GPIO_WritePin(TXPORT, TXPIN, LevelTwo);
				delay_us(length * PulseTwo);

				#ifdef RF_DEBUG
				for(int i = 0; i < PulseTwo; i++)
					if(LevelTwo == 1){
						sprintf(str_code,"%s-", str_code);
					}else{
						sprintf(str_code,"%s_", str_code);
					}
				#endif
			}
		}
	}
}



// ============================== Receive block ==================================

uint32_t cur_timestamp;
uint8_t  cur_status;
uint32_t last_change_time;
uint32_t pulse_duration;

#ifdef RF_DEBUG
uint32_t timing[512] = {0,};
uint8_t timing_count = 0;
#endif

void RX433_Int(void){

	// Calculate duration
	cur_timestamp = micros();
	cur_status = HAL_GPIO_ReadPin(RXPORT, RXPIN);
	pulse_duration = cur_timestamp - last_change_time;
	last_change_time = cur_timestamp;

#ifdef RF_DEBUG
	timing[timing_count] = pulse_duration;
	timing_count++;
	if (pulse_duration > 20000)
	{
		memset(timing, 0, sizeof(timing));
		timing_count = 0;
	}
#endif

	RF_Int(&came12);
	RF_Int(&nice12);
	RF_Int(&hcs301);

}

uint8_t RF_Int(RF_t* data){

	// Big pause reset seek code
	if (pulse_duration > data->MaxPause){
		data->PreambleCount = 0;
		return 0;
	}

	// Preamble
	if(data->PreambleCount < data->PreambleLenght){
		if((pulse_duration > data->Te - data->TeDrv) && (pulse_duration < data->Te + data->TeDrv)){
			data->PreambleCount ++;
			data->Header = 0;
			return 0;
		}else {
			data->PreambleCount = 0;
		}
	}

	// Header
	if(cur_status == (data->Invert == 0)? 0 : 1){
		if(data->PreambleCount == data->PreambleLenght && data->Header == 0){
			if((pulse_duration > data->Te * data->HeaderLenght - data->TeDrv * 4) && (pulse_duration < data->Te * data->HeaderLenght + data->TeDrv * 4)){
				data->Header = 1;
				data->BitCounter = 0;
				return 0;
			}else {
				data->PreambleCount = 0;
				data->Header = 0;
				return 0;
			}
		}
	}

	// Code
	if(cur_status == (data->Invert == 0)? 1 : 0){
		if(data->PreambleCount == data->PreambleLenght && data->Header == 1){
			if((pulse_duration > data->Te - data->TeDrv) && (pulse_duration < data->Te * 2 + data->TeDrv)){
				data->BitArray[65 - data->BitCounter] = (pulse_duration >  data->Te * 3 / 2) ? 0 : 1;
				data->BitCounter++;
				if(data->BitCounter == data->CodeLengh){
					// hсs301
					if(data->CodeLengh == 66){
						data->Repeat = data->BitArray[0];
						data->BatteryLow = data->BitArray[1];
						data->Btn2 = data->BitArray[2];
						data->Btn1 = data->BitArray[3];
						data->Btn0 = data->BitArray[4];
						data->Btn3 = data->BitArray[5];

						data->SerialNum = 0;
						for(int i = 6; i < 34; i++){
							data->SerialNum = (data->SerialNum << 1) + data->BitArray[i];
						};

						uint32_t Encrypt = 0;
						for(int i = 34; i < 66; i++){
							 Encrypt = (Encrypt << 1) + data->BitArray[i];
						};
						data->Encrypt = Encrypt;
						data->PreambleCount = 0;
					// Came, Nice 12
					}else if (data->CodeLengh == 12) {
						data->SerialNum = 0;
						for(int i = 0; i < 12; i++){
							data->SerialNum = (data->SerialNum << 1) + data->BitArray[i];
						};
					}
					if(data->TailLenght){
						return 0;
					}else {
						return 1;
					}
				}
			}
			else {
				data->PreambleCount = 0;
			}
		}
	}

	// Tail
	if(cur_status == (data->Invert == 0)? 0 : 1){
		if(data->TailLenght > 0 && data->BitCounter == data->CodeLengh){
			if((pulse_duration > data->Te * data->TailLenght - data->TeDrv * 4) && (pulse_duration < data->Te * data->TailLenght + data->TeDrv * 4)){
				return 1;
			}else {
				return 0;
			}
		}
	}
	return 0;
}

