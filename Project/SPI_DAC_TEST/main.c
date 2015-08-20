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
#include "ADC082S805.h"
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
  InitAdc082Spi();
  SPI_Cmd(SPI2, ENABLE);
  while(1){
    int i;
    for(i=0;i<256;i++){
      Set_Adc082(0,i);
      delay_ms(1);
    }
  }
}
