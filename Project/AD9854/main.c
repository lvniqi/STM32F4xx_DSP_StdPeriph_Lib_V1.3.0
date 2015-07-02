#include "main.h"
Lcd_Curve Lcd_Curve1;
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  LCD_Init();
  //LCD_Curve_Init(&Lcd_Curve1,10,10,RED);
  EXTI_init();
  int i = 1;
  AD9854_Init();
  NumBar FREQ;
  NumBar_Init(&FREQ,1,15,5,40000000,0);
  NumBar_SetValue(&FREQ,2000000);
  //LCD_ShowNumBig(0,20,LCD_STRING_LEFT,50000,RED);
  //LCD_ShowChineseStringBig(0,0,LCD_STRING_LEFT,0,3,RED);
  //LCD_ShowStringBig(LCD_STRING_WIDTH,0,LCD_STRING_RIGHT,"123ABCDE",RED);
  //LCD_ShowStringBig(LCD_STRING_WIDTH,9,LCD_STRING_RIGHT,"123abcde",RED);
  AD9854_SetSinc(FREQ.value,4000);
  while(1){
    if(MAIN_KEY.keysign){
      MAIN_KEY.keysign = 0;
      if(MAIN_KEY.keycode == 10){
        NumBar_Add(&FREQ);
        AD9854_SetSinc(FREQ.value,4000);
      }
      else if(MAIN_KEY.keycode == 14){
        NumBar_Cut(&FREQ);
        AD9854_SetSinc(FREQ.value,4000);
      }
      else if(MAIN_KEY.keycode == 13){
        NumBar_ShiftLeft(&FREQ);
      }
      else if(MAIN_KEY.keycode == 15){
        NumBar_ShiftRight(&FREQ);
      }
    }
  }
}