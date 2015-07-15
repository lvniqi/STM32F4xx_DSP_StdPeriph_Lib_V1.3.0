/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
extern "C"{
  #include "main.h"
}
Lcd_Curve Lcd_Curve1;
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  TIM2_Configuration(14);
  //¿ªÆôÆ¹ÅÒ»º³å
  PingPang_Data_Free_Init();
  ADC1_Soft_Init(ADC1,ADC_Channel_3);
  LCD_Curve_Init(&Lcd_Curve1,10,10,GREEN);
  //LCD_Write_Line(0,5,480,10,RED);
  while(1){
    if(MAIN_KEY.keysign){
      //if(MAIN_KEY.keycode == 10){
        LCD_Curve_Go_0(&Lcd_Curve1);
        for(int i=0;i<LCD_CURVE_WIDTH;i++){
          while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
          u16 t = ADC_GetConversionValue(ADC1);
          ADC_SoftwareStartConv(ADC1);
          LCD_Curve_Show(&Lcd_Curve1,t>>4,1);
          }
        }
      /*}
      else if(MAIN_KEY.keycode == 14){
        
      }
      else if(MAIN_KEY.keycode == 13){
        
      }
      else if(MAIN_KEY.keycode == 15){
        
      }
      else if(MAIN_KEY.keycode == 0){
        
      }
      else if(MAIN_KEY.keycode == 1){
        
      }*/
      MAIN_KEY.keysign = 0;
    }
}