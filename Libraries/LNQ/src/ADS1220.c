
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

//#define PGA_ON (Config_Reg[0] & ~_BV(0))
//#define PGA_OFF (Config_Reg[0] | _BV(0))

#define _BV(bit) (1<<(bit))  
//const u8 INIT_REG[4] = {0x51,0x04,0x10,0x00};
//const u8 INIT_REG[4] = {0x01,0x04,0x10,0x00};
//const u8 INIT_REG[4] = {0x01,0x04,0x50,0x00};
//const u8 INIT_REG[4] = {0x21,0x04,0x50,0x00};
//const u8 INIT_REG[4] = {0x81,0x04,0x50,0x00};
//²âµç×è
u8 INIT_REG[4] = {0x81,0x04,0x50,0x00};
//²âµçÈÝ
//u8 INIT_REG[4] = {0xA1,0x04,0x50,0x00};
const u8 CONFIG_REG_ADDRESS[4] = 
   {CONFIG_REG0_ADDRESS,
    CONFIG_REG1_ADDRESS,
    CONFIG_REG2_ADDRESS,
    CONFIG_REG3_ADDRESS};
#define ADS1220_CS SPI2_CS

void ADS1220_Write(uint8_t address, uint8_t value){
  ADS1220_CS = 0;
  SPI2_ReadWriteByte(WREG|(address<<2));      	
  SPI2_ReadWriteByte(value); 
  delay_ms(1);
  ADS1220_CS = 1;
}
void ADS1220_GPIO_Init(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
u8 ADS1220_Read(u8 address){
  u8 data;
  ADS1220_CS = 0;
  SPI2_ReadWriteByte(RREG|(address<<2));      	
  data = SPI2_ReadWriteByte(SPI_MASTER_DUMMY); 
  delay_ms(1);
  ADS1220_CS = 1;
  return data;
}
void ADS1220_Command(u8 data_in){
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
void ADS1220_PGA_ON(ADS1220* p){	 
  (p->Config_Reg[0]) &= ~_BV(0);
  ADS1220_Write(CONFIG_REG0_ADDRESS,(p->Config_Reg[0]));	
}
void ADS1220_PGA_OFF(ADS1220* p){	 
  (p->Config_Reg[0]) |= _BV(0);
  ADS1220_Write(CONFIG_REG0_ADDRESS,(p->Config_Reg[0]));	
}
void ADS1220_Continuous_Mode_ON(ADS1220* p){
  (p->Config_Reg[1]) |= _BV(2);
  ADS1220_Write(CONFIG_REG1_ADDRESS,(p->Config_Reg[1]));
}
void ADS1220_Single_Mode_ON(ADS1220* p){
  (p->Config_Reg[1]) &= ~_BV(2);
  ADS1220_Write(CONFIG_REG1_ADDRESS,(p->Config_Reg[1]));
}
void ADS1220_Set_Rate(ADS1220* p,int datarate){
  (p->Config_Reg[1]) &= ~REG_CONFIG_DR_MASK;
  switch(datarate){
    case(DR_20SPS):
      (p->Config_Reg[1]) |= REG_CONFIG_DR_20SPS; 
      break;
    case(DR_45SPS):
      (p->Config_Reg[1])|= REG_CONFIG_DR_45SPS; 
      break;
    case(DR_90SPS):
      (p->Config_Reg[1])|= REG_CONFIG_DR_90SPS; 
      break;
    case(DR_175SPS):
      (p->Config_Reg[1])|= REG_CONFIG_DR_175SPS; 
      break;
    case(DR_330SPS):
      (p->Config_Reg[1])|= REG_CONFIG_DR_330SPS; 
      break;
    case(DR_600SPS):
      (p->Config_Reg[1])|= REG_CONFIG_DR_600SPS; 
      break;
    case(DR_1000SPS):
      (p->Config_Reg[1])|= REG_CONFIG_DR_1000SPS; 
      break;
  }
  ADS1220_Write(CONFIG_REG1_ADDRESS,(p->Config_Reg[1]));
}
void ADS1220_Set_Gain(ADS1220* p,int pgagain){
  (p->Config_Reg[0]) &= ~REG_CONFIG_PGA_GAIN_MASK;
  switch(pgagain){
    case(PGA_GAIN_1):
      (p->Config_Reg[0]) |= REG_CONFIG_PGA_GAIN_1 ; 
      break;
    case(PGA_GAIN_2):
      (p->Config_Reg[0]) |= REG_CONFIG_PGA_GAIN_2; 
      break;
    case(PGA_GAIN_4):
      (p->Config_Reg[0]) |= REG_CONFIG_PGA_GAIN_4; 
      break;
    case(PGA_GAIN_8):
      (p->Config_Reg[0]) |= REG_CONFIG_PGA_GAIN_8; 
      break;
    case(PGA_GAIN_16):
      (p->Config_Reg[0]) |= REG_CONFIG_PGA_GAIN_16; 
      break;
    case(PGA_GAIN_32):
      (p->Config_Reg[0]) |= REG_CONFIG_PGA_GAIN_32; 
      break;
    case(PGA_GAIN_64):
      (p->Config_Reg[0]) |= REG_CONFIG_PGA_GAIN_64; 
      break;
    case(PGA_GAIN_128):
      (p->Config_Reg[0]) |= REG_CONFIG_PGA_GAIN_128; 
      break;
  }
  ADS1220_Write(CONFIG_REG0_ADDRESS,(p->Config_Reg[0]));
}
u32 ADS1220_Get_Config_Reg(ADS1220* p){
  u32 t;
  for(int i=0;i<4;i++){
     p->Config_Reg[i] = ADS1220_Read(CONFIG_REG_ADDRESS[i]);
  }
  t = *(u32*)p;
  return t;
}
int ADS1220_Read_Data(){
  int data = 0;
  u8*p = (u8*)&data; 
  ADS1220_CS = 0;                        //Take CS low
  delay_ms(1);
  for (int i = 2; i >= 0; i--){ 
    p[i] = SPI2_ReadWriteByte(SPI_MASTER_DUMMY);
  }
  if(data&0x00800000){
    data = (~data)+1;
    data&= 0xffffff;
    data = -data;
  }
  delay_ms(1);
  ADS1220_CS = 1;                  //  Clear CS to high
  return data;
}
void ADS1220_ChangeChannel(ADS1220* p,int pin){
  if(pin == 0){
    p->Config_Reg[0] &= 0x0F;
    p->Config_Reg[0] |= 0x80;
    ADS1220_Write( CONFIG_REG_ADDRESS[0] , p->Config_Reg[0]);
  }
  else if(pin == 2){
    p->Config_Reg[0] &= 0x0F;
    p->Config_Reg[0] |= 0xA1;
    ADS1220_Write( CONFIG_REG_ADDRESS[0] , p->Config_Reg[0]);
  }
}
void ADS1220_Init(ADS1220* p){
  InitSpi(SPI2,true);
  SPI_Cmd(SPI2,ENABLE);
  ADS1220_GPIO_Init();
  delay_ms(10);
  ADS1220_Command(RESET);                                        
  delay_ms(10);
  for(int i=0;i<4;i++){
     p->Config_Reg[i] = INIT_REG[i];
     ADS1220_Write( CONFIG_REG_ADDRESS[i] , p->Config_Reg[i]);
  }
  delay_ms(10);
  for(int i=0;i<4;i++){
     p->Config_Reg[i] = ADS1220_Read(CONFIG_REG_ADDRESS[i]);
  }
  bool check = true;
  for(int i=0;i<4;i++){
    if(p->Config_Reg[i]!= INIT_REG[i]){
      check = false;
    }
  }if(check){
    PT_DEBUG(3,"ADS1220 OK!\r\n");
  }
  else{
    PT_DEBUG(3,"ADS1220 ERROR!\r\n");
  }
  ADS1220_CS = 1;
  ADS1220_Command(START);
  delay_ms(10);
}