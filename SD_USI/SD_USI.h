#define ENABLE_BIT_DEFINITIONS

#include <ioavr.h>

#include <intrinsics.h>

#define F_CPU 8000000UL

#define _delay_ms(msecs) __delay_cycles((F_CPU/1000)*(msecs)) //simulate _delay_ms
#define _delay_us(msecs) __delay_cycles((F_CPU/1000000)*(msecs)) //simulate _delay_us

//SPI ports
#define SPI_PORT PORTB
#define SPI_DDR  DDRB 
#define SPI_PIN  PINB 


//SPI pins
#define DI 5 //pin MOSI
#define DO 6 //pin MISO
#define SCK 7 //pin SCL (SCK)
#define CS 4 //pin CS (any idle pin)
#define CS_LOW() (SPI_PORT &= ~(1<<CS))
#define CS_HIGH() (SPI_PORT |= (1<<CS))



//Test led port
#define LED_DDR DDRB
#define LED_PORT PORTB

//Test led pin
#define LED_TEST 3 

//Signature length and value
#define SIG_LEN 4
#define SIG "ZZZZ"




extern unsigned char * ret_sig;
extern unsigned char status_;
extern unsigned long int bpos;


void spi_init(void); //spi ports init
void test_init(void); //test-led port init
void test(int j); //test-led
void _spi_sendtext(char __flash *str); //send text via SPI (USI)
unsigned int spi_sendbyte(unsigned char b); //send byte via SPI (USI)
void sd_write(char __flash *str, unsigned long int bpos); //write string (up to 512 bytes) on bpos-position (div 512 bytes) SD-card
unsigned char * sd_read(unsigned long int bpos); //return SIG_LEN bytes from read block SD-card
int chk_sig(unsigned char * r, char __flash * sig); //test string for existing signature
void last_pos_sig(void); //set global bpos-var to position on SD-card without signature
void sd_init(void); //init SD-card
void sd2spi(void); //set SD-card in SPI-mode
int sd_ver(void); //check SD-card version
