#ifndef AD9854_H_
#define AD9854_H_

#include "common.h"
#include "SPI.h"
#include <stdbool.h>
typedef struct {
  u8 Config_Reg[4];
}ADS1220;
void ADS1220_Init(ADS1220* p);
#define ADS1220_DRDY_PIN PAin(7)
extern void ADS1220_Set_Gain(ADS1220* p,int pgagain);
extern void ADS1220_PGA_ON(ADS1220* p);
extern void ADS1220_PGA_OFF(ADS1220* p);
extern void ADS1220_Continuous_Mode_ON(ADS1220* p);
extern void ADS1220_Single_Mode_ON(ADS1220* p);
extern void ADS1220_Set_Rate(ADS1220* p,int datarate);
extern void ADS1220_Set_Gain(ADS1220* p,int pgagain);
extern u32 ADS1220_Get_Config_Reg(ADS1220* p); 
extern int ADS1220_Read_Data();
extern void ADS1220_Init(ADS1220* p);

#endif