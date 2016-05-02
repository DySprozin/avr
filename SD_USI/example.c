#include "SD_USI.h"



int main( void )
{
 test_init();
 
 spi_init();
 
 sd2spi();
  
 sd_ver();

 sd_init(); 
  
 last_pos_sig();

 sd_init(); 
  
 sd_write("ZZZZКу! Очередной фтагн!", bpos);
}
