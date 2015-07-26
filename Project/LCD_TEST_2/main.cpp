/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
extern "C"{
  #include "main.h"
}

int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  Lcd_Show_Line(0,10,100,10,RED);
  Lcd_Show_Line(1,8,1,100,BLUE);
  Lcd_Show_Line(0,5,100,150,WHITE);
  Lcd_Show_Line(0,150,300,0,WHITE);
  LCD_Show_Rect(0,5,100,100,WHITE);
  while(1);
}