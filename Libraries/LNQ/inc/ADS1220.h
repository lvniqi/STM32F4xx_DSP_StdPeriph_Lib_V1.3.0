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
//SPI_CPOL_Low
//SPI_CPHA_2Edge
extern void ADS1220_Set_Gain(ADS1220* p,int pgagain);
#endif