/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "SelectBar_L.h"
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  //String_L t_string(3,1,"123",LCD_STRING_MID);
  String_L t_string(3,1,0,4,RED);
  SelectItem_L t_select_Item(String_L(3,2,"CSMA",WHITE));
  NumBar_CPP t_numBar(15,0,400,0);
  t_numBar.setLtag(String_L(3,0,0,4,WHITE));
  t_numBar.setRtag(String_L(3,0,"nF",WHITE));
  t_numBar.show();
  t_numBar.setValue(500);
  t_numBar.setActive(true);
  t_select_Item.show();
  t_select_Item.setActive(true);
  t_select_Item.setBackColor(GREEN);
  t_select_Item.setActive(false);
  SelectBar_L t_select_bar(5,5,true);
  t_select_bar.show();
  while(1){
    if(MAIN_KEY.keysign){
      if(MAIN_KEY.keycode == 10){
        t_numBar.add();
        t_select_bar.addSelect(String_L(3,0,"nF",WHITE));
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
      else if(MAIN_KEY.keycode == 0){
        t_numBar.hide();
      }
      else if(MAIN_KEY.keycode == 1){
        t_numBar.show();
      }
      MAIN_KEY.keysign = 0;
    }
  }
}