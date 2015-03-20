/*
 * Stc3105.cpp
 *
 *  Created on: 25 февр. 2015 г.
 *      Author: d.kisel
 */

#include "msp430.h"
#include "types.h"
#include "i2c.h"
#include "Stc3105.h"

void Stc3105::GpioInit(void) {
	I2C_PDIR |= I2C_SDA;
	I2C_PDIR |= I2C_SCL;
	STC3105_ALM_PDIR &= ~STC3105_ALM;
	return;
}

void Stc3105::Init(void) {
	data[0] = 0x11;
	WriteBlock(1, 1, data);
	data[0] = 0x2;
	WriteBlock(1, 1, data);
	while(1) {
		ReadBlock(1, 1, data);
		if(!(data[0] & 0x2)) {
			break;
		}
	}
	data[0] = 0x10;
	data[1] = 0;
	WriteBlock(0, 2, (uint8_t *)data);
	return;
}

uint8_t Stc3105::ReadVoltage(void) {
	uint16_t voltage;

	ReadBlock(8, 2, data);
	voltage = ((uint16_t)data[1]) << 8 | data[0];
	voltage *= 2.44;
	voltage += 800;
	if(voltage < BATTERY_THR_ERR) {
		voltageLevel = BATTERY_LVL_ERROR;
	} else if(voltage > BATTERY_THR_ERR && voltage < BATTERY_THR_DEAD) {
		voltageLevel = BATTERY_LVL_DEAD;
	} else if(voltage > BATTERY_THR_DEAD && voltage < BATTERY_THR_LOW) {
		voltageLevel = BATTERY_LVL_LOW;
	} else if(voltage > BATTERY_THR_LOW && voltage < BATTERY_THR_MID) {
		voltageLevel = BATTERY_LVL_MID;
	} else if(voltage > BATTERY_THR_MID) {
		voltageLevel = BATTERY_LVL_HI;
	}
	return voltageLevel;
}

void Stc3105::Sleep(void) {
	data[0] = 0x4;
	data[1] = 0x5;
	WriteBlock(0, 1, data);
	ReadBlock(0, 2, data);
	return;
}

