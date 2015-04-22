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
#include "DDS.h"
#include "math.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
void test_gipo_init(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  LCD_Init();
  LCD_Curve_Init(&Lcd_Curve1,10,10,YELLOW);
  EXTI_init();
  InitAdc082Spi();
  SPI_Cmd(SPI2, ENABLE);
  static int i=0,j=50;
  static u32 k=1000;
  //SPI≤‚ ‘
  Set_Adc082(2,64);
  delay_ms(10);
  Set_Adc082(3,140);
  delay_ms(10);
  test_gipo_init();
  while(1){
    if(PBin(0) == 1){
      for(int i=0;i<400;i++){
        LCD_Curve_Show(&Lcd_Curve1,*(EXTERN_RAM)0x600B0002,1);
      }
      if(Lcd_Curve1.finish_flag){
        LCD_Curve_Go_0(&Lcd_Curve1);
      }
      for(int i=0;PBin(0) == 1;i++){
        LCD_Curve_Show(&Lcd_Curve1,*(EXTERN_RAM)0x600B0002,1);
      }
    }
    delay_ms(100);
    //LCD_ShowNumBig_L(400,220,60,*(EXTERN_RAM)0x600B0002,YELLOW);
  }
}
