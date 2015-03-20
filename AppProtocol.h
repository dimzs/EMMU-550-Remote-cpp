/*
 * AppProtocol.h
 *
 *  Created on: 03 марта 2015 г.
 *      Author: d.kisel
 */

#ifndef APPPROTOCOL_H_
#define APPPROTOCOL_H_

class AppProtocol {
private:
	const uint16_t *randArr;
	uint8_t randCounter;
public:
	uint8_t address;
	uint8_t channel;
	uint8_t attemptCounter;
	uint8_t statusCounter;
	uint8_t scanChannelCounter;
	uint8_t notPaired;
	uint8_t status;
	uint8_t state;
	uint8_t outState[2];
	uint8_t transferComplete[2];
	uint8_t errorCode;

	fResult operationResult;

	void Init(void);
	void RandomDelay(void);
	fResult SendRequest(CC1101 *rad);
	void ShowBattery(uint8_t value);

};



#endif /* APPPROTOCOL_H_ */
