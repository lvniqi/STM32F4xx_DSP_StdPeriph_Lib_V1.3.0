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
#include "spi.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
u8 a[32] = {1,2,3,4,5};
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  LCD_Init();
  EXTI_init();
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_InitTypeDef SPI_InitStructure;
  Spi2_GPIO_Config();
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  //两线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //CPOL=0 时钟悬空低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //CPHA=0 数据捕获第1个
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //软件NSS
  //警告！ 使用库函数时，APB2时钟为72M 而SPI需要低于18M 所以至少需要4分频
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  //4分频 18Mhz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  //CRC7 片子复位后，该位的值默认为0x07，因此将其设为7。
  SPI_Init(SPI2, &SPI_InitStructure);
  PT_DEBUG(3,"SPI2 USE MASTER!\r\n");
  SPI_Cmd(SPI2, ENABLE);
  while(1){
    PBout(12) = 0;
    SPI2_WriteByte(255);
  }
}
