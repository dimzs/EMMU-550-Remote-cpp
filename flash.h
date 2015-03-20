/*
 * flash.h
 *
 *  Created on: 25 февр. 2015 г.
 *      Author: d.kisel
 */

#ifndef FLASH_H_
#define FLASH_H_

class Msp430Flash {
private:

public:
	uint16_t ReadWord(uint16_t segmentAddress, uint16_t address);
	void WriteWord(uint16_t segmentAddress, uint16_t address, uint8_t data);
	void Erase(uint16_t segmentAddress);
	void WriteByte(uint8_t value, uint8_t offset);
	char ReadByte(uint8_t offset);
};



#endif /* FLASH_H_ */
