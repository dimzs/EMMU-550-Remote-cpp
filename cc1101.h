
#define CC1101_TX_BUFFER_LENGTH			7
#define CC1101_RX_BUFFER_LENGTH			10

#define GDO0_OUT_REG		P2OUT
#define GDO0_IN_REG			P2IN
#define GDO0_DIR_REG		P2DIR
#define GDO0_PIN			(1 << 3)
#define GDO0_IE_REG			P2IE
#define GDO0_IES_REG		P2IES
#define GDO0_IFG_REG		P2IFG

#define GDO2_OUT_REG		P2OUT
#define GDO2_IN_REG			P2IN
#define GDO2_DIR_REG		P2DIR
#define GDO2_PIN			(1 << 4)

//------------------------------------------------------------------------------------------------------
// CC2500/CC1100 STROBE, CONTROL AND STATUS REGSITER
#define CCxxx0_IOCFG2       0x00        // GDO2 output pin configuration
#define CCxxx0_IOCFG1       0x01        // GDO1 output pin configuration
#define CCxxx0_IOCFG0       0x02        // GDO0 output pin configuration
#define CCxxx0_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CCxxx0_SYNC1        0x04        // Sync word, high byte
#define CCxxx0_SYNC0        0x05        // Sync word, low byte
#define CCxxx0_PKTLEN       0x06        // Packet length
#define CCxxx0_PKTCTRL1     0x07        // Packet automation control
#define CCxxx0_PKTCTRL0     0x08        // Packet automation control
#define CCxxx0_ADDR         0x09        // Device address
#define CCxxx0_CHANNR       0x0A        // Channel number
#define CCxxx0_FSCTRL1      0x0B        // Frequency synthesizer control
#define CCxxx0_FSCTRL0      0x0C        // Frequency synthesizer control
#define CCxxx0_FREQ2        0x0D        // Frequency control word, high byte
#define CCxxx0_FREQ1        0x0E        // Frequency control word, middle byte
#define CCxxx0_FREQ0        0x0F        // Frequency control word, low byte
#define CCxxx0_MDMCFG4      0x10        // Modem configuration
#define CCxxx0_MDMCFG3      0x11        // Modem configuration
#define CCxxx0_MDMCFG2      0x12        // Modem configuration
#define CCxxx0_MDMCFG1      0x13        // Modem configuration
#define CCxxx0_MDMCFG0      0x14        // Modem configuration
#define CCxxx0_DEVIATN      0x15        // Modem deviation setting
#define CCxxx0_MCSM2        0x16        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM1        0x17        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM0        0x18        // Main Radio Control State Machine configuration
#define CCxxx0_FOCCFG       0x19        // Frequency Offset Compensation configuration
#define CCxxx0_BSCFG        0x1A        // Bit Synchronization configuration
#define CCxxx0_AGCCTRL2     0x1B        // AGC control
#define CCxxx0_AGCCTRL1     0x1C        // AGC control
#define CCxxx0_AGCCTRL0     0x1D        // AGC control
#define CCxxx0_WOREVT1      0x1E        // High byte Event 0 timeout
#define CCxxx0_WOREVT0      0x1F        // Low byte Event 0 timeout
#define CCxxx0_WORCTRL      0x20        // Wake On Radio control
#define CCxxx0_FREND1       0x21        // Front end RX configuration
#define CCxxx0_FREND0       0x22        // Front end TX configuration
#define CCxxx0_FSCAL3       0x23        // Frequency synthesizer calibration
#define CCxxx0_FSCAL2       0x24        // Frequency synthesizer calibration
#define CCxxx0_FSCAL1       0x25        // Frequency synthesizer calibration
#define CCxxx0_FSCAL0       0x26        // Frequency synthesizer calibration
#define CCxxx0_RCCTRL1      0x27        // RC oscillator configuration
#define CCxxx0_RCCTRL0      0x28        // RC oscillator configuration
#define CCxxx0_FSTEST       0x29        // Frequency synthesizer calibration control
#define CCxxx0_PTEST        0x2A        // Production test
#define CCxxx0_AGCTEST      0x2B        // AGC test
#define CCxxx0_TEST2        0x2C        // Various test settings
#define CCxxx0_TEST1        0x2D        // Various test settings
#define CCxxx0_TEST0        0x2E        // Various test settings

// Strobe commands
#define CCxxx0_SRES         0x30        // Reset chip.
#define CCxxx0_SFSTXON      0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                        // If in RX/TX: Go to a wait state where only the synthesizer is
                                        // running (for quick RX / TX turnaround).
#define CCxxx0_SXOFF        0x32        // Turn off crystal oscillator.
#define CCxxx0_SCAL         0x33        // Calibrate frequency synthesizer and turn it off
                                        // (enables quick start).
#define CCxxx0_SRX          0x34        // Enable RX. Perform calibration first if coming from IDLE and
                                        // MCSM0.FS_AUTOCAL=1.
#define CCxxx0_STX          0x35        // In IDLE state: Enable TX. Perform calibration first if
                                        // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                        // Only go to TX if channel is clear.
#define CCxxx0_SIDLE        0x36        // Exit RX / TX, turn off frequency synthesizer and exit
                                        // Wake-On-Radio mode if applicable.
#define CCxxx0_SAFC         0x37        // Perform AFC adjustment of the frequency synthesizer
#define CCxxx0_SWOR         0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define CCxxx0_SPWD         0x39        // Enter power down mode when CSn goes high.
#define CCxxx0_SFRX         0x3A        // Flush the RX FIFO buffer.
#define CCxxx0_SFTX         0x3B        // Flush the TX FIFO buffer.
#define CCxxx0_SWORRST      0x3C        // Reset real time clock.
#define CCxxx0_SNOP         0x3D        // No operation. May be used to pad strobe commands to two
                                        // bytes for simpler software.

#define CCxxx0_PARTNUM          0x30
#define CCxxx0_VERSION          0x31
#define CCxxx0_FREQEST          0x32
#define CCxxx0_LQI              0x33
#define CCxxx0_RSSI             0x34
#define CCxxx0_MARCSTATE        0x35
#define CCxxx0_WORTIME1         0x36
#define CCxxx0_WORTIME0         0x37
#define CCxxx0_PKTSTATUS        0x38
#define CCxxx0_VCO_VC_DAC       0x39
#define CCxxx0_TXBYTES          0x3A
#define CCxxx0_RXBYTES          0x3B
#define CCxxx0_RCCTRL1_STATUS   0x3C
#define CCxxx0_RCCTRL0_STATUS   0x3D

#define CCxxx0_PATABLE          			0x3E
#define CCxxx0_TXFIFO          				0x3F
#define CCxxx0_RXFIFO           			0x3F

//-------------------------------------------------------------------------------------------------------
// Definitions to support burst/single access:
#define WRITE_BURST     					0x40
#define READ_SINGLE     					0x80
#define READ_BURST      					0xC0
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
// RX Defines
#define CRC_OK              				0x80
#define RSSI                				0
#define LQI                 				1
#define BYTES_IN_RXFIFO     				0x7F
//-------------------------------------------------------------------------------------------------------
#define STATUS_STATE_MASK 					0xf0
#define STATUS_STATE_IDLE					0x0
#define STATUS_STATE_RX						0x1
#define STATUS_STATE_TX						0x2
#define STATUS_STATE_FSTXON					0x3
#define STATUS_STATE_CALIBRATE				0x4
#define STATUS_STATE_SETTLING				0x5
#define STATUS_STATE_RXFIFO_OVF				0x6
#define STATUS_STATE_TXFIFO_UDF				0x7

#define CCxxx0_GDO_HW_TO_0					0x2F
#define CCxxx0_GDO_CS						0x0E
#define CCxxx0_GDO_CCA						0x09
#define CCxxx0_GDO_WOR_EVNT0				0x24
#define CCxxx0_GDO_PKT_RCVD					0x07

#define CCxxx0_WRITE_REG_TIMEOUT			2
#define CCxxx0_READ_REG_TIMEOUT				2
#define CCxxx0_SMALL_PACKET_TX_TIMEOUT		5
#define CCxxx0_LARGE_PACKET_TX_TIMEOUT		7
#define CCxxx0_PACKET_RX_TIMEOUT			5
#define CCxxx0_RESET_TIMEOUT				30


typedef struct {
    uint8_t FSCTRL1;   // Frequency synthesizer control.
    uint8_t FSCTRL0;   // Frequency synthesizer control.
    uint8_t FREQ2;     // Frequency control word, high byte.
    uint8_t FREQ1;     // Frequency control word, middle byte.
    uint8_t FREQ0;     // Frequency control word, low byte.
    uint8_t MDMCFG4;   // Modem configuration.
    uint8_t MDMCFG3;   // Modem configuration.
    uint8_t MDMCFG2;   // Modem configuration.
    uint8_t MDMCFG1;   // Modem configuration.
    uint8_t MDMCFG0;   // Modem configuration.
    uint8_t CHANNR;    // Channel number.
    uint8_t DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    uint8_t FREND1;    // Front end RX configuration.
    uint8_t FREND0;    // Front end RX configuration.
    uint8_t MCSM0;     // Main Radio Control State Machine configuration.
    uint8_t FOCCFG;    // Frequency Offset Compensation Configuration.
    uint8_t BSCFG;     // Bit synchronization Configuration.
    uint8_t AGCCTRL2;  // AGC control.
	uint8_t AGCCTRL1;  // AGC control.
    uint8_t AGCCTRL0;  // AGC control.
    uint8_t FSCAL3;    // Frequency synthesizer calibration.
    uint8_t FSCAL2;    // Frequency synthesizer calibration.
	uint8_t FSCAL1;    // Frequency synthesizer calibration.
    uint8_t FSCAL0;    // Frequency synthesizer calibration.
    uint8_t FSTEST;    // Frequency synthesizer calibration control
    uint8_t TEST2;     // Various test settings.
    uint8_t TEST1;     // Various test settings.
    uint8_t TEST0;     // Various test settings.
    uint8_t FIFOTHR;   // RXFIFO and TXFIFO thresholds.
    uint8_t IOCFG2;    // GDO2 output pin configuration
    uint8_t IOCFG0;    // GDO0 output pin configuration
    uint8_t PKTCTRL1;  // Packet automation control.
    uint8_t PKTCTRL0;  // Packet automation control.
    uint8_t ADDR;      // Device address.
    uint8_t PKTLEN;    // Packet length.
} rfSettings_t;

typedef struct {
	uint8_t need;
	volatile uint16_t radio1;
	volatile uint16_t radio2;
	volatile uint16_t retransmission;
	volatile uint16_t tx;

} timer_t;

class CC1101 : Spi{
private:
	uint8_t ReadGdo0(void);
	uint8_t ReadGdo2(void);
public:
	uint8_t txBuffer[CC1101_TX_BUFFER_LENGTH];
	uint8_t rxBuffer[CC1101_RX_BUFFER_LENGTH];
	uint8_t packetLength;
	timer_t timer;
	void GpioInit(void);
	void GpioDeinit(void);
	void PowerupReset(void);
	fResult Reset(void);
	fResult WriteReg(uint8_t addr, uint8_t value);
	fResult WriteBurstReg(uint8_t addr, uint8_t *buffer, uint8_t count);
	uint8_t ReadReg(uint8_t addr);
	fResult ReadBurstReg(uint8_t addr, uint8_t *buffer, uint8_t count);
	uint8_t Strobe(uint8_t strobe);
	uint8_t ReadStatus(uint8_t addr);
	fResult WriteRfSettings(const rfSettings_t *rfSettings);
	fResult SendPacket(uint8_t *txBuffer, uint8_t size);
	fResult ReceivePacket(uint8_t *rxBuffer, uint8_t *length);
	fResult WaitForCCA(const uint8_t timeout);
	fResult Wake(uint8_t channel, uint8_t address);
	fResult Sleep(void);

protected:

};

