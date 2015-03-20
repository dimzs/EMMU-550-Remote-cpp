#include "msp430f2272.h"
#include "types.h"
#include "main.h"
#include "spi.h"

void Spi::SerialInterfaceSetCs(uint8_t value) {
	if(value) {
		SPI_CS_HI;
	} else {
		SPI_CS_LO;
	}
}

uint8_t Spi::SerialInterfaceReadMiso(void) {
	if(SPI_PIN & SPI_MISO) {
		return 1;
	} else {
		return 0;
	}
}
void Spi::SerialInterfaceInit(void) {
	SPI_PDIR |= SPI_CLK | SPI_MOSI;
	SPI_PDIR &= ~SPI_MISO;
	SPI_CS_PDIR |= SPI_CS;
}

void Spi::SerialInterfaceDeinit(void) {
//	SPI_PDIR &= ~SPI_CLK;
//	SPI_PDIR &= ~SPI_MISO;
//	SPI_PDIR &= ~SPI_MOSI;













//	SPI_PDIR |= SPI_CLK;
//	SPI_PDIR |= SPI_MISO;
//	SPI_PDIR |= SPI_MOSI;
//	SPI_POUT &= ~SPI_CLK;
//	SPI_POUT &= ~SPI_MISO;
//	SPI_POUT &= ~SPI_MOSI;







//	SPI_CS_PDIR |= SPI_CS;
//	SPI_CS_POUT |= SPI_CS;
	return;
}

uint8_t Spi::SerialInterfaceRw(uint8_t data) {
	uint8_t received = 0, i;

	for(i = 0; i < 8; ++i) {
		if (data & BIT7) {
			SPI_POUT |=  SPI_MOSI;   // DI = 1
		} else {
			SPI_POUT &= ~SPI_MOSI;   // DI = 0
		}
		data = (data << 1);            						// Shift bits 1 place to the left
		SPI_POUT |= SPI_CLK;                      					// SPI_CLK = 1
		received = (received << 1);                 		// Shift bits 1 place to the left
		if (SPI_PIN & SPI_MISO) {
			received = (received + 1);   					// Check digital input. If input is 'H' store a '1'
		}
		// __delay_cycles(16);
		SPI_POUT &= ~SPI_CLK;                     					// SPI_CLK = 0
	}
	return received;	
}
