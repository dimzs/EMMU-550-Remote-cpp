/*
 * main.cpp
 *
 *  Created on: 23 февр. 2015 г.
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

//#include "portmacro.h"

void ButtonsInit(void);
void hwInit(void);
void LedsInit(void);
void StartupBlink(void);






CC1101 radio;
Stc3105 battery;
Msp430Flash flash;
AppProtocol protocol;



uint8_t needStateProcessing = 0;
uint8_t eventPending = 0;
uint8_t power = 1;
uint8_t state = 0;
uint8_t event = 0;
uint16_t powerOffCounter = 0;


uint8_t attemptCounter1 = 0;
uint8_t attemptCounter2 = 0;
uint8_t batteryPending = 0;
uint16_t batteryTimeoutCounter = 0;
uint8_t batteryRefreshCounter = 0;
uint8_t batteryEnable = 0;
uint8_t batteryCounterEnabled = 0;
uint8_t batteryStopIndication = 0;


void button1Pressed_Handler(void);
void button2Pressed_Handler(void);
void button1Released_Handler(void);
void button2Released_Handler(void);
void buttonBothPressed_Handler(void);
void AckReceived_Handler(void);
void Error_Handler(void);
void SwitchToStandby_Handler(void);

void (*eventHandlers[])(void) = {
	button1Pressed_Handler,
	button2Pressed_Handler,
	button1Released_Handler,
	button2Released_Handler,
	buttonBothPressed_Handler,
	AckReceived_Handler,
	Error_Handler,
	SwitchToStandby_Handler

};

void Standby(void);
void SendingCmd(void);
void ReceivingAck(void);
void Init(void);
void Scan(void);
void Sleep(void);
void BatteryDead(void);

void (*stateProcessors[])(void) = {
	Standby,
	SendingCmd,
	ReceivingAck,
	Init,
	Scan,
	Sleep,
	BatteryDead
};

int main(void) {
	uint16_t i = 0;

	hwInit();
	__enable_interrupt();

	protocol.address = 0;
	protocol.channel = 0;
	protocol.attemptCounter = 0;
	protocol.statusCounter = 0;
	protocol.scanChannelCounter = 0;
	protocol.notPaired = 0;
	protocol.status = 0;
	protocol.state = 0;
	protocol.outState[0] = 0;
	protocol.outState[1] = 0;
	protocol.transferComplete[0] = 0;
	protocol.transferComplete[1] = 0;
	protocol.errorCode = 0;

	protocol.Init();
	protocol.address = flash.ReadByte(0);
	protocol.channel = flash.ReadByte(1);;

	if(protocol.address == 0xff || protocol.channel == 0xff) {
		protocol.address = 0;
		protocol.channel = 0;
	}

	protocol.address = 0;
	protocol.channel = 0;


	TimerEnable();

	radio.GpioInit();
	radio.PowerupReset();
	radio.Reset();
	radio.WriteRfSettings(&rfSettings);
	radio.WriteReg(CCxxx0_PATABLE, paTable);
	radio.WriteReg(CCxxx0_ADDR, protocol.address);
	radio.WriteReg(CCxxx0_CHANNR, protocol.channel);

	protocol.status = radio.Strobe(CCxxx0_SIDLE);
	while((radio.Strobe(CCxxx0_SNOP) & STATUS_STATE_MASK) != STATUS_STATE_IDLE) {
		protocol.statusCounter++;
	}

	radio.Sleep();
	battery.GpioInit();
	battery.Init();
	battery.Sleep();

	StartupBlink();

	while(1) {
		if(power) {
			if(eventPending) {																	//	Processing system events
				powerOffCounter = 0;
				(*eventHandlers[event])();
			}
			if(needStateProcessing) {
				(*stateProcessors[state])();
			} else {
//				__bis_SR_register(LPM3_bits);
			}
			if(batteryPending) {
				if(batteryEnable == 0) {
					if(!eventPending && !needStateProcessing) {
						battery.GpioInit();
						battery.Init();
						batteryEnable = 1;
						protocol.ShowBattery(battery.ReadVoltage());
						batteryPending = 0;
					}
				} else {
					if(batteryStopIndication) {
						batteryEnable = 0;
						batteryCounterEnabled = 0;
						batteryPending = 0;
						batteryStopIndication = 0;
						LED1_OFF;
						LED2_OFF;
					} else {
						protocol.ShowBattery(battery.ReadVoltage());
						batteryPending = 0;
					}
				}
			}
		} else {
//			__bis_SR_register(LPM3_bits);
		}
	}
}

void button1Pressed_Handler(void) {
	eventPending = 0;

	if(protocol.outState[0] == 0) {
		protocol.outState[0] = 1;

		radio.txBuffer[ADDRESS_BYTE] = protocol.address;
		radio.txBuffer[CHANNEL_BYTE] = protocol.channel;
		radio.txBuffer[DEVICE_TYPE_BYTE] = DEVICE_TYPE_EMMU;
		radio.txBuffer[PACKET_TYPE_BYTE] = PACKET_CMD;
		TimerEnable();

		state = STATE_INIT;
		needStateProcessing = 1;
		attemptCounter1 = 0;
		attemptCounter2 = 0;
		radio.txBuffer[CMD_CONTENT_BYTE] = CMD_ENABLE_1;

//		batteryRefreshCounter = 0;
		batteryTimeoutCounter = 0;
		batteryCounterEnabled = 1;
//		batteryEnable = 1;
	}
}

void button2Pressed_Handler(void) {
	eventPending = 0;

	if(protocol.outState[1] == 0) {
		protocol.outState[1] = 1;

		radio.txBuffer[ADDRESS_BYTE] = protocol.address;
		radio.txBuffer[CHANNEL_BYTE] = protocol.channel;
		radio.txBuffer[DEVICE_TYPE_BYTE] = DEVICE_TYPE_EMMU;
		radio.txBuffer[PACKET_TYPE_BYTE] = PACKET_CMD;
		TimerEnable();

		state = STATE_INIT;
		needStateProcessing = 1;
		attemptCounter1 = 0;
		attemptCounter2 = 0;
		radio.txBuffer[CMD_CONTENT_BYTE] = CMD_ENABLE_2;

		batteryTimeoutCounter = 0;
		batteryCounterEnabled = 1;
//		batteryEnable = 1;
	}
}

void button1Released_Handler(void) {
	eventPending = 0;

	if(protocol.outState[0] == 1) {
		protocol.outState[0] = 0;
		protocol.transferComplete[0] = 0;
		radio.timer.retransmission = 0;

		radio.txBuffer[ADDRESS_BYTE] = protocol.address;
		radio.txBuffer[CHANNEL_BYTE] = protocol.channel;
		radio.txBuffer[DEVICE_TYPE_BYTE] = DEVICE_TYPE_EMMU;
		radio.txBuffer[PACKET_TYPE_BYTE] = PACKET_CMD;
		TimerEnable();

		state = STATE_INIT;
		needStateProcessing = 1;
		attemptCounter1 = 0;
		attemptCounter2 = 0;
		radio.txBuffer[CMD_CONTENT_BYTE] = CMD_DISABLE_1;

		batteryTimeoutCounter = 0;
		batteryCounterEnabled = 1;
//		batteryEnable = 1;
	}
}

void button2Released_Handler(void) {
	eventPending = 0;

	if(protocol.outState[1] == 1) {
		protocol.outState[1] = 0;
		protocol.transferComplete[1] = 0;
		radio.timer.retransmission = 0;

		radio.txBuffer[ADDRESS_BYTE] = protocol.address;
		radio.txBuffer[CHANNEL_BYTE] = protocol.channel;
		radio.txBuffer[DEVICE_TYPE_BYTE] = DEVICE_TYPE_EMMU;
		radio.txBuffer[PACKET_TYPE_BYTE] = PACKET_CMD;
		TimerEnable();

		state = STATE_INIT;
		needStateProcessing = 1;
		attemptCounter1 = 0;
		attemptCounter2 = 0;
		radio.txBuffer[CMD_CONTENT_BYTE] = CMD_DISABLE_2;

		batteryTimeoutCounter = 0;
		batteryCounterEnabled = 1;
//		batteryEnable = 1;
	}

}

void buttonBothPressed_Handler(void) {
	eventPending = 0;

	LED1_OFF;
	LED2_OFF;
	radio.GpioInit();
	TimerEnable();
	radio.Wake(protocol.channel, protocol.address);
	radio.WriteReg(CCxxx0_PATABLE, paTable);
	radio.WriteReg(CCxxx0_PKTCTRL1, 0x07);								//	Address check & broadcast (0x0 and 0xff)
	protocol.statusCounter = 0;
// TODO: avoid infinite cycle here
	protocol.scanChannelCounter = 0;
	radio.txBuffer[ADDRESS_BYTE] = ADDRESS_BROADCAST;
	radio.txBuffer[CHANNEL_BYTE] = protocol.channel;
	radio.txBuffer[DEVICE_TYPE_BYTE] = DEVICE_TYPE_EMMU;
	radio.txBuffer[PACKET_TYPE_BYTE] = PACKET_CMD;
	radio.txBuffer[CMD_CONTENT_BYTE] = CMD_REQUEST_PAIR;
	protocol.notPaired = 1;
	state = STATE_SCAN;
	needStateProcessing = 1;
}

void AckReceived_Handler(void) {
	eventPending = 0;

}

void Error_Handler(void) {
	eventPending = 0;

}

void SwitchToStandby_Handler(void) {
	eventPending = 0;

	protocol.status = radio.Strobe(CCxxx0_SIDLE);
	while((radio.Strobe(CCxxx0_SNOP) & STATUS_STATE_MASK) != STATUS_STATE_IDLE) {
		protocol.statusCounter++;
	}
	radio.Reset();
	radio.Sleep();
	state = STATE_STANDBY;
	needStateProcessing = 1;
}

void Standby(void) {
	needStateProcessing = 0;
}

void SendingCmd(void) {
	needStateProcessing = 0;

	while(1) {
		radio.WaitForCCA(WAIT_FOR_CCA_TIMEOUT);
		protocol.RandomDelay();
		if(protocol.SendRequest(&radio) != OK) {
			protocol.errorCode = 2;
			attemptCounter1++;
			if(attemptCounter1 >= ATTEMPT_CNT_MAX) {
//				state = STATE_SWITCH_TO_STANDBY;
				event = EVENT_SWITCH_TO_STANDBY;
				eventPending = 1;
				attemptCounter1 = 0;
				break;
			}
			continue;
		} else {
			state = STATE_RECEIVING_ACK;
			needStateProcessing = 1;
			break;
		}

//		if(protocol.state == TX_STATE_SWITCH_TO_STANDBY) {
//			if(radio.txBuffer[CMD_CONTENT_BYTE] == CMD_ENABLE_1) {
//				protocol.transferComplete[0] = 1;
//				radio.timer.retransmission = 200;
//			}
//			if(radio.txBuffer[CMD_CONTENT_BYTE] == CMD_ENABLE_2) {
//				protocol.transferComplete[1] = 1;
//				radio.timer.retransmission = 200;
//			}
//			break;
//		}
	}
//	protocol.state = TX_STATE_SWITCH_TO_STANDBY;
}

void ReceivingAck(void) {
	needStateProcessing = 0;

	while(1) {
		if(radio.ReceivePacket(radio.rxBuffer, &radio.packetLength) == OK) {
			if(radio.rxBuffer[DEVICE_TYPE_BYTE - 1] == DEVICE_TYPE_EMMU && radio.rxBuffer[CHANNEL_BYTE - 1] ==  protocol.channel) {
				if(radio.rxBuffer[PACKET_TYPE_BYTE - 1] == PACKET_ACK && radio.rxBuffer[ACK_BYTE - 1] == ACK_ACKNOWLEDGED) {
					if(radio.rxBuffer[ACK_RETURNED_CMD_BYTE - 1] == radio.txBuffer[CMD_CONTENT_BYTE]) {
						event = EVENT_SWITCH_TO_STANDBY;
						eventPending = 1;
//						protocol.state = TX_STATE_SWITCH_TO_STANDBY;
						radio.Strobe(CCxxx0_SIDLE);
						attemptCounter2 = 0;
					} else {
						state = STATE_SENDING_CMD;
						needStateProcessing = 1;
						attemptCounter2++;
						if(attemptCounter2 >= ATTEMPT_CNT_MAX) {
							event = EVENT_SWITCH_TO_STANDBY;
							eventPending = 1;
							attemptCounter2 = 0;
						}
						break;
					}
				}
			}
		} else {
			attemptCounter2++;
			if(attemptCounter2 >= ATTEMPT_CNT_MAX) {
//				protocol.state = TX_STATE_SWITCH_TO_STANDBY;
				event = EVENT_SWITCH_TO_STANDBY;
				eventPending = 1;
				attemptCounter2 = 0;
			} else {
				state = STATE_SENDING_CMD;
				needStateProcessing = 1;
			}
			break;
		}
	}
}

void Init(void) {
	needStateProcessing = 0;

	ButtonsInit();
	radio.GpioInit();
	radio.Wake(protocol.channel, protocol.address);
	protocol.statusCounter = 0;
// TODO: avoid infinite cycle here
	while((radio.Strobe(CCxxx0_SNOP) & STATUS_STATE_MASK) != STATUS_STATE_IDLE) {
		protocol.statusCounter++;
	}
	state = STATE_SENDING_CMD;
	needStateProcessing = 1;
	protocol.attemptCounter = 0;
}

void Scan(void) {
	needStateProcessing = 0;

	while(1) {
		delay_us(100000);
		radio.WriteReg(CCxxx0_CHANNR, protocol.scanChannelCounter);
		protocol.scanChannelCounter++;
		if(protocol.scanChannelCounter > NUMBER_OF_CHANNELS - 1) {
			protocol.scanChannelCounter = 0;
			break;
		}
		radio.Strobe(CCxxx0_SIDLE);
		radio.WaitForCCA(WAIT_FOR_CCA_TIMEOUT);
		protocol.RandomDelay();
		radio.txBuffer[0] = sizeof(radio.txBuffer) - 1;
		if(radio.SendPacket(radio.txBuffer, sizeof(radio.txBuffer))) {

		} else {
			if(!radio.ReceivePacket(radio.rxBuffer, &radio.packetLength)) {
				if(radio.rxBuffer[DEVICE_TYPE_BYTE - 1] == DEVICE_TYPE_EMMU) {
					if(radio.rxBuffer[PACKET_TYPE_BYTE - 1] == PACKET_ACK && radio.rxBuffer[ACK_BYTE - 1] == ACK_REQUEST_BIND_ACCEPTED && radio.rxBuffer[ADDRESS_BYTE - 1] == ADDRESS_BROADCAST) {
						protocol.address = radio.rxBuffer[ACK_RETURNED_CMD_BYTE - 1];
						radio.Strobe(CCxxx0_SIDLE);
						protocol.state = TX_STATE_SWITCH_TO_STANDBY;
						protocol.channel = radio.ReadReg(CCxxx0_CHANNR);
						protocol.scanChannelCounter = 0;
						radio.WriteReg(CCxxx0_PKTCTRL1, 0x05);
						flash.Erase(0x1000);
						flash.WriteByte(protocol.channel, 1);
						flash.WriteByte(protocol.address, 0);
						LED1_OFF;
						LED2_ON;
						delay_ms(200);
						LED2_OFF;
						delay_ms(200);
						LED2_ON;
						delay_ms(200);
						LED2_OFF;
						delay_ms(200);
						LED2_ON;
						delay_ms(200);
						LED2_OFF;
						delay_ms(1000);
						protocol.notPaired = 0;
						break;
					} else {

					}
				} else {

				}
			} else {

			}
		}
	}
	if(protocol.notPaired) {
		protocol.notPaired = 0;
		LED2_OFF;
		LED1_ON;
		delay_ms(200);
		LED1_OFF;
		delay_ms(200);
		LED1_ON;
		delay_ms(200);
		LED1_OFF;
		delay_ms(200);
		LED1_ON;
		delay_ms(200);
		LED1_OFF;
		delay_ms(1000);
	}
	radio.WriteReg(CCxxx0_PKTCTRL1, 0x05);
	event = EVENT_SWITCH_TO_STANDBY;
	eventPending = 1;
}

void Sleep(void) {

}

void BatteryDead(void) {

}

void hwInit(void) {

//	BCSCTL1 = CALBC1_16MHZ;                 // Set range
//	DCOCTL = CALDCO_16MHZ;                  // Set DCO step + modulation*/
//	WDTCTL = WDT_ADLY_250;                   // WDT 1s/4 interval timer
//	IE1 |= WDTIE;

// WDT
	WDTCTL = WDTPW + WDTHOLD;				// Stop watchdog timer to prevent time out reset
	DCOCTL = 0;                             // Select lowest DCOx and MODx settings
//	BCSCTL1 = CALBC1_16MHZ;                 // Set range
//	DCOCTL = CALDCO_16MHZ;                  // Set DCO step + modulation*/
	BCSCTL1 = CALBC1_1MHZ;                 // Set range
	DCOCTL = CALDCO_1MHZ;                  // Set DCO step + modulation*/

	// Timer A
	TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
	TACCR0 = 327;
	TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode

	FCTL2 = FWKEY + FSSEL_2 + FN0 + FN1 + FN2 + FN3 + FN4 + FN5;



//	BCSCTL3 |= LFXT1S_2;

	P1OUT =  0x00;
	P2OUT =  0x00;
	P3OUT =  0x00;
	P4OUT =  0x00;
	P1DIR = 0xff;
	P2DIR = 0xff;
	P3DIR = 0xff;
	P4DIR = 0xff;

//	Stc3105_GpioInit();
//	CC1101_GpioInit();
	LedsInit();
	ButtonsInit();

//// Timer A
//	TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
//	TACCR0 = 327;
//	TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode


//	// Timer_A is configured for up mode, thus the the timer overflows when TAR counts to CCR0.
//	CCTL0 = CCIE;                   // CCR0 interrupt enabled
//	// CCTL1 = OUTMOD_7;            // CCR1 reset/set
//	CCR0  = 120;                  	// PWM Period		100 Hz
//	CCR1  =     0;                  // CCR1 PWM duty cycle
//	// TACTL = TASSEL_2 + MC_1 + ID_3; // SMCLK, upmode, divider: 3 - /8
//	TACTL = TASSEL_1 + MC_1; // ACLK, upmode, divider: /1
}

void ButtonsInit(void) {
	BTN_SEL &= ~BTN1_PIN;					// All pins as GPIO
	BTN_SEL &= ~BTN2_PIN;
	BTN_DIR &= ~BTN1_PIN;   				// In
	BTN_DIR &= ~BTN2_PIN;
	BTN_OUT &= ~BTN1_PIN;					// P1 reset
	BTN_OUT &= ~BTN2_PIN;
	BTN_IE |= BTN1_PIN | BTN2_PIN;
	BTN_IES |= BTN1_PIN | BTN2_PIN;
	BTN_IFG &= ~BTN1_PIN;
	BTN_IFG &= ~BTN2_PIN;
	return;
}

void LedsInit(void) {
	LED1_PSEL &= ~LED1;
	LED1_PDIR |= LED1;
	LED1_POUT &= ~LED1;
	LED2_PSEL &= ~LED2;
	LED2_PDIR |= LED2;
	LED2_POUT &= ~LED2;
	return;
}

void StartupBlink(void) {
	LED1_ON;
	delay_ms(500);
	LED2_ON;
	delay_ms(500);
	LED1_OFF;
	delay_ms(500);
	LED2_OFF;
	return;
}
