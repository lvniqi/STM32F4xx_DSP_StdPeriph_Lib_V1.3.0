/* Includes ------------------------------------------------------------------*/
#include "main.h"
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  Init();
  int t_before;
  PT* thread_1 = mymalloc(sizeof(PT));
  PT_INIT(thread_1,20,PRINT_TEST);
  PT_ADD_THREAD(thread_1);
  //SelectBar sb;
  //SelectBar_Init(&sb,1,5,8);
  //LCD_STRING t;
  //t.type = _LCD_STRING_CHINESE;
  /*t.string.chinese.start =4;
  t.string.chinese.len =4;
  SelectBar_AddSelect(&sb,t);
  t.string.chinese.start =8;
  SelectBar_AddSelect(&sb,t);
  t.string.chinese.start =12;
  SelectBar_AddSelect(&sb,t);
  t.string.chinese.start =16;
  
  SelectBar_AddSelect(&sb,t);*/
  while(1){
    PT_SERVICE();
    if(MAIN_KEY.keysign){
      if(MAIN_KEY.keycode == 5){
          SelectBar_ShiftUp(&sub_menu);
        //SelectBar_ShiftUp(&main_menu);
        //NumBar_Add(&RES);
      }
      else if(MAIN_KEY.keycode == 9){
          SelectBar_ShiftDown(&sub_menu);
        //SelectBar_ShiftUp(&main_menu);
        //NumBar_Add(&RES);
      }
      else if(MAIN_KEY.keycode == 8){
        SelectBar_ShiftDown(&main_menu);
        //NumBar_Cut(&RES);
      }
      else if(MAIN_KEY.keycode == 4){
        SelectBar_ShiftUp(&main_menu);
        //SelectBar_Hide(&main_menu);
        //NumBar_Hide(&CAP);
        //NumBar_Show(&RES);
        //NumBar_ShiftLeft(&RES);
      }
      else if(MAIN_KEY.keycode == 15){
        //SelectBar_Show(&main_menu);
        //NumBar_ShiftRight(&RES);
        //NumBar_Hide(&RES);
        //NumBar_Show(&CAP);
      }
      MAIN_KEY.keysign = 0;
    }
  }
}