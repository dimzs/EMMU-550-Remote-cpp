#include <msp430f2272.h>
#include <stdbool.h>
#include "types.h"
#include "main.h"
#include "spi.h"
#include "cc1101.h"
#include "reg_settings.h"

extern const rfSettings_t rfSettings;
extern const uint8_t paTable;

void CC1101::PowerupReset(void) {
//	SerialInterfaceSetCs(1);					//NSSMD0 = 1;
//	delay_us(1);								//halWait(1);
//	SerialInterfaceSetCs(0);					//NSSMD0 = 0;
//	delay_us(1);								//halWait(1);
//	SerialInterfaceSetCs(1);					//NSSMD0 = 1;
//	delay_us(41);								//halWait(41);
//	Reset();
//	return;

	SPI_CS_HI;					//NSSMD0 = 1;
	delay_us(1);				//halWait(1);
	SPI_CS_LO;					//NSSMD0 = 0;
	delay_us(1);				//halWait(1);
	SPI_CS_HI;					//NSSMD0 = 1;
	delay_us(41);				//halWait(41);
	Reset();
	return;
}

void CC1101::GpioInit(void) {
	SerialInterfaceInit();
	GDO0_DIR_REG &= ~GDO0_PIN;
	GDO2_DIR_REG &= ~GDO2_PIN;
}

void CC1101::GpioDeinit(void) {
//	SpiDeinit();
}

uint8_t CC1101::ReadGdo0(void) {
	if(GDO0_IN_REG & GDO0_PIN) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t CC1101::ReadGdo2(void) {
	if(GDO2_IN_REG & GDO2_PIN) {
		return 1;
	} else {
		return 0;
	}
}

fResult CC1101::Reset(void) {
	SPI_CS_LO;
//	SerialInterfaceSetCs(0);					//NSSMD0 = 0;		Pull  CSn  low
	timer.radio1 = CCxxx0_RESET_TIMEOUT;
	while (SerialInterfaceReadMiso()) {			// 					and wait for  SO  to go low (CHIP_RDYn).
		if(timer.radio1 == 0) {
			return TIMEOUT;
		}
	}
	SerialInterfaceRw(CCxxx0_SRES);				//SPI0DAT = CCxxx0_SRES;
	timer.radio1 = CCxxx0_RESET_TIMEOUT;
	while (SerialInterfaceReadMiso()) {
	if(timer.radio1 == 0) {
			return TIMEOUT;
		}
	}
	SPI_CS_HI;
//	SerialInterfaceSetCs(1);					//NSSMD0 = 1;
	return OK;
}

fResult CC1101::WriteReg(uint8_t addr, uint8_t value) {
	SPI_CS_LO;
//	SerialInterfaceSetCs(0);
	timer.radio1 = CCxxx0_WRITE_REG_TIMEOUT;
	while (SPI_PIN & SPI_MISO) {
		if(timer.radio1 == 0) {
			return TIMEOUT;
		}
	}
	SerialInterfaceRw(addr);
	SerialInterfaceRw(value);
	SPI_CS_HI;
//	SerialInterfaceSetCs(1);
	return OK;
}

fResult CC1101::WriteBurstReg(uint8_t addr, uint8_t *buffer, uint8_t count) {
	uint8_t i;

	timer.radio1 = CCxxx0_WRITE_REG_TIMEOUT;
	SPI_CS_LO;
//	SerialInterfaceSetCs(0);
	while (SPI_PIN & SPI_MISO) {
		if(timer.radio1 == 0) {
			return TIMEOUT;
		}
	}
	SerialInterfaceRw(addr | WRITE_BURST);
	for (i = 0; i < count; i++) {
		SerialInterfaceRw(buffer[i]);
	}
	SPI_CS_HI;
//	SerialInterfaceSetCs(1);
	return OK;
}

uint8_t CC1101::ReadReg(uint8_t addr) {
    uint8_t data;
	
    timer.radio1 = CCxxx0_READ_REG_TIMEOUT;
    SerialInterfaceSetCs(0);
	while (SerialInterfaceReadMiso()) {
		if(timer.radio1 == 0) {
			return 0;
		}
	}
	SerialInterfaceRw(addr | READ_SINGLE);
	data = SerialInterfaceRw(0);
	SerialInterfaceSetCs(1);
    return data;
}

fResult CC1101::ReadBurstReg(uint8_t addr, uint8_t *buffer, uint8_t count) {
    uint8_t i;
	
    timer.radio1 = CCxxx0_READ_REG_TIMEOUT;
    SerialInterfaceSetCs(0);
    while (SerialInterfaceReadMiso()) {
		if(timer.radio1 == 0) {
			return TIMEOUT;
		}
	}
    SerialInterfaceRw(addr | READ_BURST);
    for (i = 0; i < count; i++) {
        buffer[i] = SerialInterfaceRw(0);
    }
    SerialInterfaceSetCs(1);
	return OK;
}

uint8_t CC1101::Strobe(uint8_t strobe) {
	uint8_t status = 0;

	timer.radio1 = CCxxx0_WRITE_REG_TIMEOUT;
	SPI_CS_LO;
//	SerialInterfaceSetCs(0);
    while (SerialInterfaceReadMiso() && timer.radio1);
    status = SerialInterfaceRw(strobe);
//    SerialInterfaceSetCs(1);
    SPI_CS_HI;
	return status;
}

uint8_t CC1101::ReadStatus(uint8_t addr) {
    uint8_t data;
	
    timer.radio1 = CCxxx0_READ_REG_TIMEOUT;
    SerialInterfaceSetCs(0);
    while (SerialInterfaceReadMiso() && timer.radio1);
    SerialInterfaceRw(addr | READ_BURST);
    data = SerialInterfaceRw(0);
    SerialInterfaceSetCs(1);
    return data;
}

fResult CC1101::SendPacket(uint8_t *txBuffer, uint8_t size) {
	if(size <= 6) {
		timer.radio2 = CCxxx0_SMALL_PACKET_TX_TIMEOUT;
	} else {
		timer.radio2 = CCxxx0_LARGE_PACKET_TX_TIMEOUT;
	}
    if(WriteBurstReg(CCxxx0_TXFIFO, txBuffer, size)) {
		return TIMEOUT;
	}
    Strobe(CCxxx0_STX);
    // Wait for GDO0 to be set -> sync transmitted
	while(!(GDO0_IN_REG & GDO0_PIN)) {
		if(timer.radio2 == 0) {
			return TIMEOUT;
		}
	}

    // Wait for GDO0 to be cleared -> end of packet
    while(GDO0_IN_REG & GDO0_PIN) {
		if(timer.radio2 == 0) {
			return TIMEOUT;
		}
	}
	return OK;
}

fResult CC1101::ReceivePacket(uint8_t *rxBuffer, uint8_t *length) {
    uint8_t status[2];
    uint8_t packetLength;

    timer.radio2 = CCxxx0_PACKET_RX_TIMEOUT;
//	timer2 = 10;
    Strobe(CCxxx0_SRX);
	// Wait for GDO0 to be set -> sync received
	while(!ReadGdo0()) {
		if(timer.radio2 == 0) {
			return TIMEOUT;
		}
	}
    // Wait for GDO0 to be cleared -> end of packet
    while(ReadGdo0()) {
		if(timer.radio2 == 0) {
			return TIMEOUT;
		}
	}

    // This status register is safe to read since it will not be updated after
    // the packet has been received (See the CC1100 and 2500 Errata Note)
    if ((ReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) {

        // Read length byte
        packetLength = ReadReg(CCxxx0_RXFIFO);
    
        // Read data from RX FIFO and store in rxBuffer
        if (packetLength <= *length) {
            if(ReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength)) {
				return NOT_RECEIVED;
			} 
            *length = packetLength;
        
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            ReadBurstReg(CCxxx0_RXFIFO, status, 2);
        
            // MSB of LQI is the CRC_OK bit
            // return (status[LQI] & CRC_OK);
			return OK;
        } else {
            *length = packetLength;

            // Make sure that the radio is in IDLE state before flushing the FIFO
            // (Unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point) 
            Strobe(CCxxx0_SIDLE);

            // Flush RX FIFO
            Strobe(CCxxx0_SFRX);
            return NOT_RECEIVED;
        }
    } else
        return NOT_RECEIVED;
}

fResult CC1101::WaitForCCA(const uint8_t timeout) {
	WriteReg(CCxxx0_IOCFG2, CCxxx0_GDO_CCA);
	Strobe(CCxxx0_SRX);
	timer.radio1 = timeout;
	while(!ReadGdo2()) {
		if(timer.radio1 == 0) {
			return TIMEOUT;
		}
	}
	Strobe(CCxxx0_SFRX);
	Strobe(CCxxx0_SIDLE);
	return OK;
}

fResult CC1101::Wake(uint8_t channel, uint8_t address) {
	Reset();
	WriteRfSettings(&rfSettings);
	WriteReg(CCxxx0_PATABLE, paTable);
	WriteReg(CCxxx0_CHANNR, channel);
	WriteReg(CCxxx0_ADDR, address);
	Strobe(CCxxx0_SCAL);
	timer.tx = 10;
	while((Strobe(CCxxx0_SNOP) & STATUS_STATE_MASK) != STATUS_STATE_IDLE) {
		if(timer.tx == 0) {
			return TIMEOUT;
		}
	}
	return OK;
}

fResult CC1101::Sleep(void) {
	WriteReg(CCxxx0_IOCFG2, CCxxx0_GDO_HW_TO_0);
	WriteReg(CCxxx0_IOCFG0, CCxxx0_GDO_HW_TO_0);
	WriteReg(CCxxx0_MCSM0, (ReadReg(CCxxx0_MCSM0) & (~1)));
	Strobe(CCxxx0_SNOP);
	Strobe(CCxxx0_SIDLE);
	timer.tx = 10;
	while((Strobe(CCxxx0_SNOP) & STATUS_STATE_MASK) != STATUS_STATE_IDLE) {
		if(timer.tx == 0) {
			return TIMEOUT;
		}
	}
	Strobe(CCxxx0_SPWD);
	return OK;
}

fResult CC1101::WriteRfSettings(const rfSettings_t *rfSettings) {
	int result = OK;

    // Write register settings
	result |= WriteReg(CCxxx0_FSCTRL1,  rfSettings->FSCTRL1);
	result |= WriteReg(CCxxx0_FSCTRL0,  rfSettings->FSCTRL0);
	result |= WriteReg(CCxxx0_FREQ2,    rfSettings->FREQ2);
	result |= WriteReg(CCxxx0_FREQ1,    rfSettings->FREQ1);
	result |= WriteReg(CCxxx0_FREQ0,    rfSettings->FREQ0);
	result |= WriteReg(CCxxx0_MDMCFG4,  rfSettings->MDMCFG4);
	result |= WriteReg(CCxxx0_MDMCFG3,  rfSettings->MDMCFG3);
	result |= WriteReg(CCxxx0_MDMCFG2,  rfSettings->MDMCFG2);
	result |= WriteReg(CCxxx0_MDMCFG1,  rfSettings->MDMCFG1);
	result |= WriteReg(CCxxx0_MDMCFG0,  rfSettings->MDMCFG0);
	result |= WriteReg(CCxxx0_CHANNR,   rfSettings->CHANNR);
	result |= WriteReg(CCxxx0_DEVIATN,  rfSettings->DEVIATN);
	result |= WriteReg(CCxxx0_FREND1,   rfSettings->FREND1);
	result |= WriteReg(CCxxx0_FREND0,   rfSettings->FREND0);
	result |= WriteReg(CCxxx0_MCSM0 ,   rfSettings->MCSM0 );
	result |= WriteReg(CCxxx0_FOCCFG,   rfSettings->FOCCFG);
	result |= WriteReg(CCxxx0_BSCFG,    rfSettings->BSCFG);
	result |= WriteReg(CCxxx0_AGCCTRL2, rfSettings->AGCCTRL2);
	result |= WriteReg(CCxxx0_AGCCTRL1, rfSettings->AGCCTRL1);
	result |= WriteReg(CCxxx0_AGCCTRL0, rfSettings->AGCCTRL0);
	result |= WriteReg(CCxxx0_FSCAL3,   rfSettings->FSCAL3);
	result |= WriteReg(CCxxx0_FSCAL2,   rfSettings->FSCAL2);
	result |= WriteReg(CCxxx0_FSCAL1,   rfSettings->FSCAL1);
	result |= WriteReg(CCxxx0_FSCAL0,   rfSettings->FSCAL0);
	result |= WriteReg(CCxxx0_FSTEST,   rfSettings->FSTEST);
	result |= WriteReg(CCxxx0_TEST2,    rfSettings->TEST2);
	result |= WriteReg(CCxxx0_TEST1,    rfSettings->TEST1);
	result |= WriteReg(CCxxx0_TEST0,    rfSettings->TEST0);
	result |= WriteReg(CCxxx0_FIFOTHR,  rfSettings->FIFOTHR);
	result |= WriteReg(CCxxx0_IOCFG2,   rfSettings->IOCFG2);
	result |= WriteReg(CCxxx0_IOCFG0,   rfSettings->IOCFG0);
	result |= WriteReg(CCxxx0_PKTCTRL1, rfSettings->PKTCTRL1);
	result |= WriteReg(CCxxx0_PKTCTRL0, rfSettings->PKTCTRL0);
	result |= WriteReg(CCxxx0_ADDR,     rfSettings->ADDR);
	result |= WriteReg(CCxxx0_PKTLEN,   rfSettings->PKTLEN);
	return (fResult)result;
}
