//#define PxSEL				P1SEL // Port selection
//#define PxDIR 				P1DIR // Port direction
//#define PxOUT 				P1OUT // Port output
//#define PxIN  				P1IN  // Port input

#define SPI_PSEL			P1SEL // Port selection
#define SPI_PDIR 			P1DIR // Port direction
#define SPI_POUT 			P1OUT // Port output
#define SPI_PIN  			P1IN  // Port input
#define SPI_CS_PSEL			P3SEL
#define SPI_CS_PDIR			P3DIR
#define SPI_CS_POUT			P3OUT
#define SPI_CLK				BIT2
#define SPI_MOSI			BIT3
#define SPI_MISO			BIT0
#define SPI_CS				BIT7


//#define SPI_CS_IN			P3IN
//
//#define SEL   				BIT7  // Controls SEL line
//#define CLK   				BIT2  // Controls CLK line
//#define SI    				BIT3  // Controls DI  line
//#define SO    				BIT0  // Controls DO  line вход

#define  SPI_WRITE_HI		SPI_POUT |=  SPI_MOSI
#define  SPI_WRITE_LO		SPI_POUT &= ~SPI_MOSI
#define  SPI_MOSI_HI		SPI_POUT |=  SPI_MOSI
#define  SPI_MOSI_LO		SPI_POUT &= ~SPI_MOSI
#define  SPI_CLOCK_HI		SPI_POUT |=  SPI_CLK
#define  SPI_CLOCK_LO		SPI_POUT &= ~SPI_CLK
#define  SPI_CS_HI			SPI_CS_POUT |=  SPI_CS
#define  SPI_CS_LO			SPI_CS_POUT &= ~SPI_CS
//#define  SPI_SEL_HI			P3OUT |=  BIT7
//#define  SPI_SEL_LO			P3OUT &= ~BIT7

//#define MOSI				SI
//#define MISO				SO

class Spi {
private:

public:
	void SerialInterfaceSetCs(uint8_t value);
	uint8_t SerialInterfaceReadMiso(void);
	void SerialInterfaceInit(void);
	void SerialInterfaceDeinit(void);
	uint8_t SerialInterfaceRw(uint8_t data);

protected:

};

