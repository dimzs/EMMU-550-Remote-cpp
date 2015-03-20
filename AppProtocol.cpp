/*
 * AppProtocol.cpp
 *
 *  Created on: 03 марта 2015 г.
 *      Author: d.kisel
 */

#include <msp430.h>
#include <stdint.h>
#include "main.h"
#include "spi.h"
#include "cc1101.h"
#include "i2c.h"
#include "Stc3105.h"
#include "reg_settings.h"
#include "flash.h"
#include "AppProtocol.h"

const uint16_t randdd[] = {447, 220, 325, 349, 238, 381, 441, 331, 418, 392, 254, 108, 97, 306, 357, 386, 113, 292, 10, 304, 216, 265, 275, 159, 47, 355, 233, 62, 50, 466, 114, 458, 267, 486, 195, 352, 279, 66, 48, 303, 23, 360, 478, 119, 250, 95, 49, 262, 308, 314};

void AppProtocol::Init(void) {
	randArr = randdd;
}

void AppProtocol::RandomDelay(void) {
	uint16_t i;

	for(i = 0; i < randArr[randCounter]; i++) {
		delay_us(1);
	}
	randCounter++;
	if(randCounter >= 50) {
		randCounter = 0;
	}

	return;
}

fResult AppProtocol::SendRequest(CC1101 *rad) {

	operationResult = OK;
	rad->packetLength = sizeof(rad->txBuffer);
	rad->txBuffer[0] = rad->packetLength - 1;
	attemptCounter = 0;
	do {
		operationResult = rad->SendPacket(rad->txBuffer, rad->packetLength);
		attemptCounter++;
		if(attemptCounter >= SEND_REQUEST_ATTEMPTS_NUM) {
			operationResult = TIMEOUT;
			break;
		}
	} while(operationResult);

	return operationResult;
}

void AppProtocol::ShowBattery(uint8_t value) {
	switch(value) {
	case BATTERY_LVL_ERROR:

		break;
	case BATTERY_LVL_DEAD:

		break;
	case BATTERY_LVL_LOW:
		LED1_ON;
		LED2_OFF;
		break;
	case BATTERY_LVL_MID:
		LED1_ON;
		LED2_ON;
		break;
	case BATTERY_LVL_HI:
		LED1_OFF;
		LED2_ON;
		break;
	}
	return;
}






