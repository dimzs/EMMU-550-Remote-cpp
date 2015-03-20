/*
 * flash.cpp
 *
 *  Created on: 25 февр. 2015 г.
 *      Author: d.kisel
 */

#include "msp430.h"
#include "types.h"
#include "flash.h"

uint16_t Msp430Flash::ReadWord(uint16_t segmentAddress, uint16_t address) {
	uint16_t *fp;

	fp = (uint16_t *) segmentAddress;
	fp += address;					// add offset

	return (*fp);
}

void Msp430Flash::WriteWord(uint16_t segmentAddress, uint16_t address, uint8_t data) {
	uint8_t *fp;

	fp = (uint8_t *) segmentAddress;
	fp += address;             														// add offset
	FCTL1 = FWKEY + WRT;            												// Set WRT bit for write operation
	FCTL3 = FWKEY;                  												// Clear Lock bit
	*fp = data;            															// Write value to flash
	FCTL1 = FWKEY;                  												// Clear WRT bit
	FCTL3 = FWKEY + LOCK;           												// Reset LOCK bit

	return;
}

void Msp430Flash::Erase(uint16_t segmentAddress) {
	uint16_t *fp;

	fp = (uint16_t *) segmentAddress;
	FCTL1 = FWKEY + ERASE;          												// Set Erase bit
	FCTL3 = FWKEY;                  												// Clear Lock bit
	*fp = 0;                 														// Dummy write to erase Flash segment
	FCTL1 = FWKEY;                  												// Clear WRT bit
	FCTL3 = FWKEY + LOCK;           												// Reset LOCK bit

	return;
}

void Msp430Flash::WriteByte(uint8_t value, uint8_t offset) {
	uint8_t *Flash_ptr;                          // Flash pointer

	Flash_ptr = (uint8_t *)0x1000 + offset;               // Initialize Flash pointer
	FCTL3 = FWKEY;                            // Clear Lock bit
	FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
	*Flash_ptr = value;                   // Write value to flash

	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

char Msp430Flash::ReadByte(uint8_t offset) {
	uint8_t *fp;

	fp = (uint8_t *) 0x1000 + offset;

	return (*fp);
}
