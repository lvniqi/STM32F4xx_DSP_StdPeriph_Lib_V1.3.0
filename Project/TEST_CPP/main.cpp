/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  NumBar_CPP t_numBar(0,0,400,1);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  t_numBar.show();
  t_numBar.setValue(500);
  t_numBar.setActive(true);
  while(1){
    if(MAIN_KEY.keycode == 10){
        t_numBar.add();
      }
      else if(MAIN_KEY.keycode == 14){
        t_numBar.cut();
      }
      else if(MAIN_KEY.keycode == 13){
        t_numBar.shiftLeft();
      }
      else if(MAIN_KEY.keycode == 15){
        t_numBar.shiftRight();
      }
  }
}