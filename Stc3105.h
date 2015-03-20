/*
 * Stc3105.h
 *
 *  Created on: 25 февр. 2015 г.
 *      Author: d.kisel
 */

#ifndef STC3105_H_
#define STC3105_H_

//#define BATTERY_LED_TIMER		800
//#define BATTERY_LED_TIMER		400
//#define BATTERY_REFRESH_TIMER	100

#define BATTERY_LVL_ERROR		0
#define BATTERY_LVL_DEAD		1
#define BATTERY_LVL_LOW			2
#define BATTERY_LVL_MID			3
#define BATTERY_LVL_HI			4

#define BATTERY_THR_ERR			2900
#define BATTERY_THR_DEAD		3100
#define BATTERY_THR_LOW			3600
#define BATTERY_THR_MID			3800

typedef struct {
	volatile uint16_t timer;
	volatile uint16_t refreshTimer;
	uint8_t refreshFlag;
	uint8_t flag;
	uint8_t lowVoltage;
	uint16_t voltage;
	uint8_t voltageLevel;
	uint8_t init;
	uint8_t deepSleep;
	uint8_t dead;
} battery_t;

class Stc3105 : SoftI2c {
private:
	uint8_t data[2];
public:
	uint8_t voltageLevel;
	void Init(void);
	void GpioInit(void);
	uint8_t ReadVoltage(void);
	void Sleep(void);
};



#endif /* STC3105_H_ */
