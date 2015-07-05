#ifndef AD9854_H_
#define AD9854_H_

#include "common.h"
#include "SPI.h"
#include <stdbool.h>
typedef struct {
  u8 Config_Reg0;
  u8 Config_Reg1;
  u8 Config_Reg2;
  u8 Config_Reg3;
}ADS1220;
void ADS1220_Init(ADS1220* p);
//SPI_CPOL_Low
//SPI_CPHA_2Edge
#endif