
#include "ADS1220.h"

#define SPI_MASTER_DUMMY   0xFF
#define RESET 0x06                                                    //Send the RESET command (06h) to make sure the ADS1220 is properly reset after power-up
#define START 0x08                                         //Send the START/SYNC command (08h) to start converting in continuous conversion mode
#define WREG  0x40
#define RREG  0x20
#define CONFIG_REG0_ADDRESS 0x00
#define CONFIG_REG1_ADDRESS 0x01
#define CONFIG_REG2_ADDRESS 0x02
#define CONFIG_REG3_ADDRESS 0x03


#define REG_CONFIG_DR_20SPS 0x00
#define REG_CONFIG_DR_45SPS 0x20
#define REG_CONFIG_DR_90SPS 0x40
#define REG_CONFIG_DR_175SPS 0x60
#define REG_CONFIG_DR_330SPS 0x80
#define REG_CONFIG_DR_600SPS 0xA0
#define REG_CONFIG_DR_1000SPS 0xC0
#define REG_CONFIG_DR_MASK 0xE0

#define DR_20SPS 20
#define DR_45SPS 45
#define DR_90SPS 90
#define DR_175SPS 175
#define DR_330SPS 330
#define DR_600SPS 600
#define DR_1000SPS 1000

#define REG_CONFIG_PGA_GAIN_1 0x00
#define REG_CONFIG_PGA_GAIN_2 0x02
#define REG_CONFIG_PGA_GAIN_4 0x04
#define REG_CONFIG_PGA_GAIN_8 0x06
#define REG_CONFIG_PGA_GAIN_16 0x08
#define REG_CONFIG_PGA_GAIN_32 0x0A
#define REG_CONFIG_PGA_GAIN_64 0x0C
#define REG_CONFIG_PGA_GAIN_128 0x0E
#define REG_CONFIG_PGA_GAIN_MASK 0x0E

#define PGA_GAIN_1 1
#define PGA_GAIN_2 2
#define PGA_GAIN_4 4
#define PGA_GAIN_8 8
#define PGA_GAIN_16 16
#define PGA_GAIN_32 32
#define PGA_GAIN_64 64
#define PGA_GAIN_128 128

//#define PGA_ON (Config_Reg0 & ~_BV(0))
//#define PGA_OFF (Config_Reg0 | _BV(0))

#define ADS1220_CS_PIN 7
#define ADS1220_DRDY_PIN 6
#define _BV(bit) (1<<(bit))  

#define ADS1220_CS SPI2_CS

void ADS1220_Write(uint8_t address, uint8_t value){
  ADS1220_CS = 0;
  delay_ms(5);
  SPI2_ReadWriteByte(WREG|(address<<2));      	
  SPI2_ReadWriteByte(value); 
  delay_ms(5);
  ADS1220_CS = 1;
}

u8 ADS1220_Read(u8 address){
  u8 data;
  ADS1220_CS = 0;
  delay_ms(5);
  SPI2_ReadWriteByte(RREG|(address<<2));      	
  data = SPI2_ReadWriteByte(SPI_MASTER_DUMMY); 
  delay_ms(5);
  ADS1220_CS = 1;

  return data;
}
void ADS1220_Command(u8 data_in)
{
  ADS1220_CS = 0;
  delay_ms(2);
  ADS1220_CS = 1;
  delay_ms(2);
  ADS1220_CS = 0;
  delay_ms(2);
  SPI2_ReadWriteByte(data_in);
  delay_ms(2);
  ADS1220_CS = 1;
}
void ADS1220_Init(ADS1220* p){
  static char data;
  //SPI.begin();                           // wake up the SPI bus.
  //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE1);
  InitSpi(SPI2,true);
  SPI_Cmd(SPI2,ENABLE);
	
  delay_ms(100);
  ADS1220_Command(RESET);                                        
  delay_ms(100);                  
  /*SPI.transfer(WREG);           //WREG command (43h, 08h, 04h, 10h, and 00h)
  SPI.transfer(0x01);      	
  SPI.transfer(0x04);     
  SPI.transfer(0x10);    
  SPI.transfer(0x00);   
  */
  p->Config_Reg0 = 0x01;
  p->Config_Reg1 = 0x04;
  p->Config_Reg2 = 0x10;
  p->Config_Reg3 = 0x00;
  

  ADS1220_Write( CONFIG_REG0_ADDRESS , p->Config_Reg0);
  ADS1220_Write( CONFIG_REG1_ADDRESS , p->Config_Reg1);
  ADS1220_Write( CONFIG_REG2_ADDRESS , p->Config_Reg2);
  ADS1220_Write( CONFIG_REG3_ADDRESS , p->Config_Reg3);
  delay_ms(100);
  /*
  SPI.transfer(RREG);           //RREG
  data = SPI.transfer(SPI_MASTER_DUMMY);
  //Serial.println(data);
  data = SPI.transfer(SPI_MASTER_DUMMY); 
  //Serial.println(data);
  data = SPI.transfer(SPI_MASTER_DUMMY); 
  //Serial.println(data);
  data = SPI.transfer(SPI_MASTER_DUMMY); 
  //Serial.println(data);
  */

  p->Config_Reg0 = ADS1220_Read(CONFIG_REG0_ADDRESS);
  p->Config_Reg1 = ADS1220_Read(CONFIG_REG1_ADDRESS);
  p->Config_Reg2 = ADS1220_Read(CONFIG_REG2_ADDRESS);
  p->Config_Reg3 = ADS1220_Read(CONFIG_REG3_ADDRESS);

  if(p->Config_Reg0 == 0x01&&
     p->Config_Reg1 == 0x04&&
     p->Config_Reg2 == 0x10&&
     p->Config_Reg3 == 0x00){
    PT_DEBUG(4,"ADS1220 OK!\r\n");
   }
  else{
    PT_DEBUG(4,"ADS1220 ERROR!\r\n");
  }
  ADS1220_CS = 1;
  ADS1220_Command(START);
  delay_ms(100);

}