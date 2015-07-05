/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#include "USART.h"
#include "AD.h"
#include "DA.h"
#include "Timer.h"
#include "Pingpang.h"
#include "stm32f4xx_dac.h"
#include <stdbool.h>
#include "spi.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
u8 a[32] = {1,2,3,4,5};
//ADS1246?????  
#define ADC_CMD_WAKEUP      0x00            //??????  
#define ADC_CMD_SLEEP       0x02            //??????  
#define ADC_CMD_SYNC        0x04            //??ADC??  
#define ADC_CMD_RESET       0x06            //????  
#define ADC_CMD_NOP     0xFF            //???  
#define ADC_CMD_RDATA       0x12            //??????  
#define ADC_CMD_RDATAC      0x14            //??????  
#define ADC_CMD_SDATAC      0x16            //??????  
#define ADC_CMD_RREG        0x20            //????  
#define ADC_CMD_WREG        0x40            //????  
#define ADC_CMD_SYSOCAL     0x60            //??????  
#define ADC_CMD_SYSGCAL     0x61            //??????  
#define ADC_CMD_SELFOCAL    0x62            //?????  
#define ADC_CMD_RESTRICTED  0xF1            //  
void ADS1246_WriteCmd(u8 Cmd)  
{  
    SPI2_CS = 0;  
    SPI2_ReadWriteByte(Cmd);  
    SPI2_CS = 1;
}  
void ADS1220_ReadReg(u8 RegAddr,u8 *Buffer,u8 Length)  {
    u8 Cmd[2];  
    SPI2_CS = 0;  
    Cmd[0]=ADC_CMD_RREG|RegAddr;  
    Cmd[1]=Length-1;  
    SPI2_ReadWriteByte(Cmd[1]);  
    ADC_ReadBytes(Buffer,Length);  
    Cmd[0]=ADC_CMD_NOP;  
    SPI2_ReadWriteByte(Cmd[0]);  
    SPI2_CS = 1;  
} 
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  LCD_Init();
  InitSpi(SPI2,true);
  SPI_Cmd(SPI2,ENABLE);
  SPI2_CS = 0;
  SPI2_ReadWriteByte(ADC_CMD_RESET);
  while(1){  
    SPI2_ReadWriteByte(ADC_CMD_RDATA);
    SPI2_ReadWriteByte(0xff);
    SPI2_ReadWriteByte(0xff);
    SPI2_ReadWriteByte(0xff);
    SPI2_ReadWriteByte(0xff);
  }
}
