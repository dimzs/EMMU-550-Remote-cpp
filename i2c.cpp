#include "msp430.h"
#include "types.h"
#include "i2c.h"

unsigned int    g_uiSleep;    // Счетчик для задержки
unsigned int    gv_uiSleepCnt;// Счетчик для задержки

void Sleep(unsigned int uiSleep) //#############################################
{ while(--uiSleep); // Sleep(4*100) <- Задержка на 100 микросек (100 us)
} // Sleep() //     //                 на частоте 16 МГц


void usSleep(unsigned int uiSleep) //###########################################
{                                  // Задержка на uiSleep микросек (us)
  Sleep(uiSleep);                  // на частоте 16 МГц
  Sleep(uiSleep);
  Sleep(uiSleep);
  Sleep(uiSleep);
} // usSleep() //

void msSleep(unsigned int uiSleep) //###########################################
{                                  // Задержка на uiSleep миллисек (ms)
  for(gv_uiSleepCnt=1; gv_uiSleepCnt<=uiSleep; gv_uiSleepCnt++)
  { Sleep(4000);
  }
} // msSleep() //



#define NUM_BYTES_TX 1                      // Bytes to write,
                                            //this value must equal to "NUM_BYTES" that define in msp430x22x4_uscib0_i2c_13.c
#define NUM_BYTES_RX 30                      // Bytes to read

int RXByteCtr, RPT_Flag = 0;                // enables repeated start when 1
volatile unsigned int counter = 0;
// volatile unsigned char RxBuffer[128];       // Allocate 128 byte of RAM
unsigned char RxBuffer[64], TxBuffer[64];
unsigned char *PTxData;                     // Pointer to TX data
unsigned char *PRxData;                     // Pointer to RX data
unsigned char TXByteCtr;
unsigned char MSData = 0;

void Setup_TX(void);
void Setup_RX(void);
void Transmit(void);
void Receive(void);
void I2CWriteInit(void);
void I2CReadInit(void);

void SoftI2c::DelayUs(uint16_t value) {
	usSleep(value);
	return;
}

void SoftI2c::DelayMs(uint16_t value) {
	msSleep(value);
	return;
}

void SoftI2c::Start(void) {
	I2C_PDIR |= SDA;         // Set to output, SDA = 0
	DelayUs(2*T);         // Delay
	I2C_PDIR |= SCL;         // Set to output, SCL = 0
	DelayUs(2*T);         // Delay
	return;
}
void SoftI2c::Stop(void) {
	I2C_PDIR |= SDA;        // Set to output, data low [SCA = 0]
	DelayUs(2*T);        // Delay
	I2C_PDIR &= ~SCL;       // Set to input, SCL = 1 via pull-up
	DelayUs(2*T);        // Delay
	I2C_PDIR &= ~SDA;       // Set to input, SDA = 1 via pull-up
}

void SoftI2c::GetAck(void) //#####################################################
{                     // Set up for I2C acknowledge
   I2C_PDIR &= ~SCL;     // Set to input, SCL = 1 via pull-up
// I2C_PDIR &= ~SDA;     // Set to input, SDA = 1 via pull-up
   DelayUs(4*T);      // Delay
// !!! ТУТ НАДО АНАЛИЗИРОВАТЬ ASK !!!
   I2C_PDIR |= SCL;      // Set to output, data low [SCL = 0]
// I2C_PDIR |= SDA;      // Set to output, SDA = 1 via pull-up

   DelayUs(4*T);      // Delay
} // SftI2C_GetAck() //

void SoftI2c::SetAck(void) //#####################################################
{                     // Set up for I2C acknowledge
   I2C_PDIR |= SDA;      // Set to output, SDA = 0
   DelayUs(T);        // Delay

   I2C_PDIR &= ~SCL;     // Set to input, SCL = 1 via pull-up
   DelayUs(2*T);      // Delay
   I2C_PDIR |= SCL;      // Set to output, SCL = 0
   DelayUs(T);        // Delay
   I2C_PDIR &= ~SDA;     // Set to input, SDA = 1 via pull-up

   DelayUs(4*T);      // Delay
} // SftI2C_SetAck() //

void SoftI2c::TxByte(uint8_t data) {
	uint8_t bits;

	bits = 0x08;                         // Load I2C bit counter
	while (bits != 0x00) {                 // Loop until all bits are shifted
		if (data & BIT7) {
			I2C_PDIR &= ~SDA;     // Set to input, SDA = 1 via pull-up
		} else {
			I2C_PDIR |= SDA;     // Set to output, data low [SDA = 0]
		}
		DelayUs(T);                        // Delay
		I2C_PDIR &= ~SCL;                     // Set to output, data low [SCL = 0]
		data = (data << 1);                // Shift bits 1 place to the left
		DelayUs(T);                        // Delay
		I2C_PDIR |= SCL;                      // Set to output, data low [SCL = 0]
		DelayUs(T);                        // Delay
		bits--;                            // Loop until 8 bits are sent
	}
	I2C_PDIR &= ~SDA;                       // Set to input, SDA = 1 via pull-up
	DelayUs(T);
	GetAck();                     		// Send acknowledge
}

uint8_t SoftI2c::RxByte(uint8_t ack) {
	uint8_t bits, temp = 0;        // I2C bit counter

	bits = 0x08;                         // Load I2C bit counter
	while (bits > 0)                     // Loop until all bits are read
	{
		DelayUs(T);                        // Delay
		I2C_PDIR &= ~SCL;                     // Set to input, SDL = 1 via pull-up
		temp = (temp << 1);                // Shift bits 1 place to the left
		if (I2C_PIN & SDA) {
			temp = (temp + 1); // Check digital input. If input is 'H' store a '1'
		}
		DelayUs(T);                        // Delay
		I2C_PDIR |= SCL;                      // Set to output, data low [SCL = 0]
		DelayUs(T);                        // Delay
		bits--;                            // Decrement I2C bit counter
	}
	DelayUs(T);

	if(ack) {
		GetAck();        // Send acknowledge
	} else {
		SetAck();
	}
	return temp;
}

void SoftI2c::WriteBlock(uint8_t address, uint8_t length, uint8_t *data) {
	uint8_t i;

	Start();
	TxByte(0xE0);
	TxByte(address);
	for (i = 0; i < length; i++) {
		TxByte(*(data));    // Send data and ack
		data++;                    // Increment pointer to next element
	}
	Stop();                  // Send Stop condition
	DelayMs(15);                    // Задержка для записи
}

void SoftI2c::ReadBlock(uint8_t address, uint8_t length, uint8_t *data) {
	uint8_t i;

	Start();
	TxByte(0xE0);
	TxByte(address);   // Адрес в NVM
	Stop();                  // Send Stop condition
	DelayUs(4*T);
	Start();
	TxByte(0xE1);
	for (i = 1; i < length; i++) {
		*(data) = RxByte(0);// Выставляем ASK принудительно
		data++;                    // Increment pointer to next element
	}
	*(data) = RxByte(1); // Не выставляем ПОСЛЕДНИЙ ASK
	Stop();                  // Send Stop condition
}


