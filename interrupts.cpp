/*
 * interrupts.cpp
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: d.kisel
 */

#include <msp430.h>
#include <stdint.h>
#include "main.h"
#include "spi.h"
#include "i2c.h"
#include "Stc3105.h"
#include "cc1101.h"
#include "reg_settings.h"
#include "flash.h"
#include "AppProtocol.h"

extern CC1101 radio;
extern Stc3105 battery;
extern Msp430Flash flash;
extern AppProtocol protocol;

extern uint8_t power;
extern uint8_t state;
extern uint8_t event;
extern uint8_t eventPending;
extern uint8_t batteryPending;
extern uint16_t batteryTimeoutCounter;
extern uint8_t batteryRefreshCounter;
extern uint8_t batteryCounterEnabled;
extern uint8_t batteryStopIndication;
extern uint8_t batteryEnable;

//void __attribute__ ((interrupt(PORT2_VECTOR))) Port2_ISR (void) {
#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void) {
	if(BTN_IFG & BTN1_PIN) {
		if(BTN_IN & BTN1_PIN) {
			BTN_IES |= BTN1_PIN;
			event = EVENT_BUTTON_1_RELEASED;
			eventPending = 1;
			power = 1;
		} else {
			BTN_IES &= ~BTN1_PIN;
			event = EVENT_BUTTON_1_PRESSED;
			eventPending = 1;
			power = 1;
			if(!(BTN_IN & BTN2_PIN)) {
				event = EVENT_BUTTON_BOTH_PRESSED;
			}
		}
		BTN_IFG &= ~BTN1_PIN;
	} else if(BTN_IFG & BTN2_PIN) {
		if(BTN_IN & BTN2_PIN) {
			BTN_IES |= BTN2_PIN;
			event = EVENT_BUTTON_2_RELEASED;
			eventPending = 1;
			power = 1;
		} else {
			BTN_IES &= ~BTN2_PIN;
			event = EVENT_BUTTON_2_PRESSED;
			eventPending = 1;
			power = 1;
			if(!(BTN_IN & BTN1_PIN)) {
				event = EVENT_BUTTON_BOTH_PRESSED;
			}
		}
		BTN_IFG &= ~BTN2_PIN;
	} else {
		BTN_IFG = 0;
	}

//	if(battery.deepSleep) {
//		__bic_SR_register_on_exit(LPM4_bits);
//		battery.deepSleep = 0;
//	} else {
//		__bic_SR_register_on_exit(LPM3_bits);
//	}

	__bic_SR_register_on_exit(LPM4_bits);
}

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA_ISR(void)
{
	if(radio.timer.radio1) {
		radio.timer.radio1--;
	}
	if(radio.timer.radio2) {
		radio.timer.radio2--;
	}
	if(radio.timer.tx) {
		radio.timer.tx--;
	}
	if(batteryCounterEnabled) {
		if(!batteryPending) {
			batteryRefreshCounter++;
			if(batteryRefreshCounter >= 100) {
				batteryPending = 1;
				batteryRefreshCounter = 0;
			}
		}
	}
	if(batteryEnable) {
		batteryTimeoutCounter++;
		if(batteryTimeoutCounter > 500) {
//			batteryEnable = 0;
			batteryStopIndication = 1;
			batteryTimeoutCounter = 0;
			batteryRefreshCounter = 0;
			batteryPending = 0;
		}
	}
//	if(battery.timer) {
//		battery.timer--;
//		if(battery.timer == 0) {
//			if(!outputState[0] && !outputState[1]) {
//				battery.flag = 1;
//				__bic_SR_register_on_exit(LPM3_bits);
//			}
//		}
//	}
//	if(battery.refreshTimer) {
//		battery.refreshTimer--;
//		if(battery.refreshTimer == 0) {
//			battery.refreshTimer = BATTERY_REFRESH_TIMER;
//			battery.refreshFlag = 1;
//			__bic_SR_register_on_exit(LPM3_bits);
//		}
//	}


//	if(protocol.outState[0] && protocol.transferComplete[0]) {
//		if(radio.timer.retransmission) {
//			radio.timer.retransmission--;
//		}
//		if(radio.timer.retransmission == 0) {
//			protocol.state = TX_STATE_INIT;
//			protocol.transferComplete[0] = 0;
//			__bic_SR_register_on_exit(LPM4_bits);
//		}
//	}
//	if(protocol.outState[1] && protocol.transferComplete[1]) {
//		if(radio.timer.retransmission) {
//			radio.timer.retransmission--;
//		}
//		if(radio.timer.retransmission == 0) {
//			protocol.state = TX_STATE_INIT;
//			protocol.transferComplete[1] = 0;
//			__bic_SR_register_on_exit(LPM4_bits);
//		}
//	}
}



