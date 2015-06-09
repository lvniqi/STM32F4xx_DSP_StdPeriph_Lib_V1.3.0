/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
u32 count = 0;
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  LCD_Init();
  LCD_Curve_Init(&Lcd_Curve1,10,10,GREEN);
  EXTI_init();
  while(SD_Init() != SD_OK){
    LCD_ShowStringBig(120,100,"ERROR!",WHITE);
    LCD_ShowStringBig(40,60,"PRESS ANY KEY TO INGORE!",WHITE);
  }
  
  while (1){
  }
}

