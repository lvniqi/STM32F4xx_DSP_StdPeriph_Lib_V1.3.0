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
  ADC1_Init(ADC_Channel_3);
  LCD_Curve_Init(&Lcd_Curve1,10,10,GREEN);
  while(1){
    Ad_Get_Service();
    _pingpang_data *temp = PingPang_Out(&pingpang_ad);
    if(temp){
      PingPang_Free(temp);
    }
    if(MAIN_KEY.keysign){
      //if(MAIN_KEY.keycode == 10){
        temp = PingPang_Out(&pingpang_ad);
        while(temp == NULL){
          Ad_Get_Service();
          temp = PingPang_Out(&pingpang_ad);
          }
        LCD_Curve_Go_0(&Lcd_Curve1);
        for(int i=0;i<LCD_CURVE_WIDTH;i++){
          LCD_Curve_Show(&Lcd_Curve1,(temp->data[i])>>4,1);
          }
        PingPang_Free(temp);
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