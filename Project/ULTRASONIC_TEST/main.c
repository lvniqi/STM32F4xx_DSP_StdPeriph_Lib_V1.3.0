/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */

Lcd_Curve Lcd_Curve1;
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  UR_GPIO_Configuration();
  UR_NVIC_Configuration();
  UR_TIM_Configuration();
  EXTI_init();
  LCD_Init();
  LCD_Clear1(WHITE);
  BACK_COLOR = WHITE;
  int i,j;
  for(i=0;i<3;i++){
    for(j=1;j<4;j++){
      LCD_Show_Rect_FULL(i*80+10,j*80,40,40,BLUE);
    }
  }
  while(1){
    PEout(4) = 1;
    delay_ms(1);
    PEout(4) = 0;
    delay_ms(100);
  }
}


