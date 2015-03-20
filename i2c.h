#define I2C_PSEL 				P3SEL // Port selection
#define I2C_PDIR 				P3DIR // Port direction
#define I2C_POUT 				P3OUT // Port output
#define I2C_PIN  				P3IN  // Port input
#define I2C_SDA					(1 << 1)
#define I2C_SCL					(1 << 2)

#define STC3105_ALM_POUT		P3OUT
#define STC3105_ALM_PDIR		P3DIR
#define STC3105_ALM_PIN			P3IN
#define STC3105_ALM				(1 << 0)

#define SDA   					BIT1  // Controls SDA line (pull-up used for logic 1)
#define SCL   					BIT2  // Controls SCL line (pull-up used for logic 1)
#define T        				5  // <- Задержка

class SoftI2c {
private:
	void DelayUs(uint16_t value);
	void DelayMs(uint16_t value);
public:
	void Start(void);
	void Stop(void);
	void GetAck(void);
	void SetAck(void);
	void TxByte(uint8_t data);
	uint8_t RxByte(uint8_t ack);
	void WriteBlock(uint8_t address, uint8_t length, uint8_t *data);
	void ReadBlock(uint8_t address, uint8_t length, uint8_t *data);
};
