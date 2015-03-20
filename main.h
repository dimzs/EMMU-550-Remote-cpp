#define delay_us(n)					__delay_cycles(n)
#define delay_ms(n)					__delay_cycles((unsigned long)n*1000)
//#define delay_us(n)					__delay_cycles(n*16)
//#define delay_ms(n)					__delay_cycles((unsigned long)n*16000)
#define TimerEnable()				TACTL |= MC_1
#define TimerDisable()				TACTL &= ~MC_1


#define LED1_ON						LED1_POUT |= LED1
#define LED1_OFF					LED1_POUT &= ~LED1
#define LED1_TOGGLE					LED1_POUT ^= LED1
#define LED2_ON						LED2_POUT |= LED2
#define LED2_OFF					LED2_POUT &= ~LED2
#define LED2_TOGGLE					LED2_POUT ^= LED2

#define LED1_POUT					P3OUT
#define LED1_PIN					P3IN
#define LED1_PDIR					P3DIR
#define LED1_PSEL					P3SEL
#define LED1						(1 << 3)
#define LED2_POUT					P3OUT
#define LED2_PIN					P3IN
#define LED2_PDIR					P3DIR
#define LED2_PSEL					P3SEL
#define LED2						(1 << 6)

#define BTN_SEL						P2SEL
#define BTN_DIR						P2DIR
#define BTN_OUT						P2OUT
#define BTN_IN						P2IN
#define BTN_IE						P2IE
#define BTN_IES						P2IES
#define BTN_IFG						P2IFG

#define BTN1_PIN					(1 << 0)
#define BTN2_PIN					(1 << 1)

#define TX_STATE_STANDBY			0
#define TX_STATE_SENDING_CMD		1
#define TX_STATE_RECEIVING_ACK		2
#define TX_STATE_ACK_RECEIVED		3
#define TX_STATE_ERROR				4
#define TX_STATE_INIT				5
#define TX_STATE_SWITCH_TO_STANDBY	6
#define TX_STATE_SCAN				7
#define TX_STATE_SLEEP				8
#define TX_STATE_BATTERY_DEAD		9


#define STATE_STANDBY			0
#define STATE_SENDING_CMD		1
#define STATE_RECEIVING_ACK		2
#define STATE_INIT				3
#define STATE_SCAN				4
#define STATE_SLEEP				5
#define STATE_BATTERY_DEAD		6

#define EVENT_BUTTON_1_PRESSED		0
#define EVENT_BUTTON_2_PRESSED		1
#define EVENT_BUTTON_1_RELEASED		2
#define EVENT_BUTTON_2_RELEASED		3
#define EVENT_BUTTON_BOTH_PRESSED	4
#define EVENT_ACK_RECEIVED			5
#define EVENT_ERROR					6
#define EVENT_SWITCH_TO_STANDBY		7


#define SCAN_STATE_INIT				0
#define SCAN_STATE_SENDING_CMD		1
#define SCAN_STATE_RECEIVING_ACK	2
#define SCAN_STATE_ACK_RECEIVED		3

#define PACKET_SIZE_BYTE			0
#define CHANNEL_BYTE				2
#define ADDRESS_BYTE				1
#define DEVICE_TYPE_BYTE			3
#define PACKET_TYPE_BYTE			4
#define CMD_CONTENT_BYTE			5
#define ACK_BYTE					5
#define ACK_RETURNED_CMD_BYTE		6
#define PAIR_ADDRESS_BYTE			6

#define DEVICE_TYPE_EMMU			0x37

//#define DEVICE_TYPE_EMMU			0x82

#define PACKET_CMD					0x1
#define PACKET_ACK					0x2

#define CMD_ENABLE_1				0x01
#define CMD_DISABLE_1				0x02
#define CMD_ENABLE_2				0x03
#define CMD_DISABLE_2				0x04
#define CMD_REQUEST_PAIR			0xf0

#define ACK_ACKNOWLEDGED			0xCC
#define ACK_NOT_ACKNOWLEDGED		0x1
#define ACK_REQUEST_RETRANSMISSION	0x2
#define ACK_REQUEST_BIND_ACCEPTED	0xf0

#define ADDRESS						0x13
#define CHANNEL						0x3
#define ADDRESS_BROADCAST			0xff
#define ATTEMPT_CNT_MAX				10

#define TX_RETRANSMISSION_TIMEOUT	200
#define RX_RETRANSMISSION_TIMEOUT	300
#define RX_CALIBRATION_PERIOD		100			// *10 ms
#define WAIT_FOR_CCA_TIMEOUT		10
#define WAIT_FOR_RESPONSE_TIMEOUT	10


#define SEND_REQUEST_ATTEMPTS_NUM	5
#define NUMBER_OF_CHANNELS			5

#define TIMER_NEEDED_BY_RADIO		(1 << 0)





typedef enum {
	OK = 0,
	TIMEOUT,
	ERROR,
	NOT_RECEIVED,
	NOT_READY,
	DENIED,
	NOT_ENABLED,
	LOCKED,
	INVALID_PARAMETER
} fResult;


void ButtonsInit(void);
void LedsInit(void);
void hwInit(void);
fResult SendRequest(uint8_t *txBuffer, uint8_t length);
unsigned int FlashReadWord(unsigned int segmentAddress, unsigned int address);
void FlashWriteWord(unsigned int segmentAddress, unsigned int address, unsigned char data);
void FlashErase(unsigned int segmentAddress);
void FlashWriteByte(char value, char offset);
char FlashReadByte(char offset);
uint16_t MeasureTemp(void);
void StartupBlink(void);




