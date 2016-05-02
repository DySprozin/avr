#include "SD_USI.h"


unsigned char * ret_sig;
unsigned char status_;
unsigned long int bpos;

void spi_init() {
  SPI_DDR |=  (1<<SCK) | (1<<CS) | (0<<DI) | (1<<DO);
}

void test_init(void) {
  LED_DDR |= (1<<LED_TEST);
  LED_PORT |= (1<<LED_TEST);
  _delay_ms(1000);
  LED_PORT &= ~(1<<LED_TEST);
}

void test(int j) {
  for (int i = 0; i<j; i++) {
   LED_PORT |= (1<<LED_TEST);  
   _delay_ms(100);
   LED_PORT &= ~(1<<LED_TEST); 
    _delay_ms(100);
  }
}


void sd2spi(void) {
 CS_HIGH();
  
  for (int i = 0; i < 10; i++) {
    spi_sendbyte(0xFF);
  }
  
  CS_LOW();
  
   //command CDM0
  spi_sendbyte(0x40); //index command cmd0
  spi_sendbyte(0x00); //first arg's byte
  spi_sendbyte(0x00); //2-nd arg's byte
  spi_sendbyte(0x00); //3-rd arg's byte
  spi_sendbyte(0x00); //4-th arg's byte
  spi_sendbyte(0x95); // CRC
  while ((status_=spi_sendbyte(0xFF))!=0x01);
  test(4);
}

int sd_ver(void) {
  int ver;
  spi_sendbyte(0x48); //index command cmd8
  spi_sendbyte(0x00); //first arg's byte
  spi_sendbyte(0x00); //2-nd arg's byte
  spi_sendbyte(0x01); //3-rd arg's byte
  spi_sendbyte(0xAA); //4-th arg's byte
  spi_sendbyte(0X00); // CRC
    
  do { 
      status_=spi_sendbyte(0xFF);
        
      // SD v.1 
      if (status_==0x04 || status_==0x05) { 
       ver = 1;
      }                
      // SD v.2
      if (status_==0x01) { 
       ver = 2;       
      }
      
  } while((status_!=0x01) && (status_!=0x04) && (status_!=0x05));
  
  return ver;
}


unsigned int spi_sendbyte(unsigned char b) {

  USIDR=b;
  USISR|=(1<<USIOIF);
  
  while((USISR&(1<<USIOIF))==0){
   USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC); 
   _delay_us(10);
  }
  
 _delay_us(100);
 return USIDR;
}

void _spi_sendtext(char __flash *str) {
  int i = 0;
  while (str[i]) {
    spi_sendbyte(str[i++]);
  }
}



void sd_write(char __flash *str, unsigned long int bpos) {
  CS_LOW();
  
  spi_sendbyte(0xFF);
  
  //converting bpos to 4 bytes and set write-start position
  spi_sendbyte(0x58);
  spi_sendbyte(((bpos & 0xFF000000)>>24));
  spi_sendbyte(((bpos & 0x00FF0000)>>16)); 
  spi_sendbyte(((bpos & 0x0000FF00)>>8));
  spi_sendbyte(((bpos & 0x000000FF)));
  spi_sendbyte(0x00); //CRC
 
  while (spi_sendbyte(0xFF)!=0x0);
  
  spi_sendbyte(0xFF); 
  spi_sendbyte(0xFE); 
  _spi_sendtext(str); //str len up to 512 bytes
  spi_sendbyte(0x00); //1-st byte CRC
  spi_sendbyte(0x00); //2-nd byte CRC
  
  while (spi_sendbyte(0xFF)&0x5 != 0x5); //waiting for the command reception signal

  while (spi_sendbyte(0xFF)!=0x00); //waiting for the command reception signal
  
  test(2);
  
  CS_HIGH();
  
}



unsigned char * sd_read(unsigned long int bpos) { 
  CS_LOW();
  
  spi_sendbyte(0xFF);
  
  //converting bpos to 4 bytes and set read-start position
  
  spi_sendbyte(0x51);
  spi_sendbyte(((bpos & 0xFF000000)>>24));
  spi_sendbyte(((bpos & 0x00FF0000)>>16)); 
  spi_sendbyte(((bpos & 0x0000FF00)>>8));
  spi_sendbyte(((bpos & 0x000000FF)));
  spi_sendbyte(0x00); //CRC
 

  while (spi_sendbyte(0xFF)!=0x0);
  
  while (spi_sendbyte(0xFF)!=0xFE); //waiting for byte of the transmission start signal
  

  for (int i=0; i<SIG_LEN; i++) {
    ret_sig[i] = spi_sendbyte(0xFF);
  }
  
  for (int i=0; i<(512-SIG_LEN); i++) {
    spi_sendbyte(0xFF);
  }
  

  spi_sendbyte(0x00); //1-st byte CRC
  spi_sendbyte(0x00); //2-nd byte CRC
  
  CS_HIGH();
  test(2);
  return ret_sig;
  
}

int chk_sig(unsigned char * r, char __flash * sig) {
  for (int i = 0; i < SIG_LEN; i++) {
    if (r[i] != sig[i]) return 0;
  }
  return 1;
}

void last_pos_sig(void) {
  while (chk_sig(sd_read(bpos), SIG)) {
    sd_init();
    bpos += 512;
  }
}

void sd_init(void) {
  CS_LOW();

  while (1) {
   spi_sendbyte(0x41); //CMD1
   spi_sendbyte(0x00);
   spi_sendbyte(0x00);
   spi_sendbyte(0x00);
   spi_sendbyte(0x00);
   spi_sendbyte(0xFF);
   spi_sendbyte(0xFF); //byte waiting
   status_=spi_sendbyte(0xFF); //check status
   if (status_ != 0xFF) {
    if (status_ == 0x0) break;
   }
  }

  CS_HIGH();
}

