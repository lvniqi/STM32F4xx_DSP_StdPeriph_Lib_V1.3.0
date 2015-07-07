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
  /*SelectItem si;
  SelectItem_Init(&si,1,5,5);
  SelectItem_Show(&si);
  SelectItem_SetActive(&si,true);
  SelectItem_SetActive(&si,false);
  LCD_STRING t;
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "CS1";
  SelectItem_SetString(&si,t);*/
  SelectBar sb;
  SelectBar_Init(&sb,1,5,5);
  LCD_STRING t;
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "CS1";
  SelectBar_AddSelect(&sb,t);
  t.string.ascii = "CS2";
  SelectBar_AddSelect(&sb,t);
  t.string.ascii = "CS3";
  SelectBar_AddSelect(&sb,t);
  t.string.ascii = "CS4";
  SelectBar_AddSelect(&sb,t);
  t.string.ascii = "TITLE";
  SelectBar_SetTitle(&sb,t);
  SelectBar_Show(&sb);
  SelectItem_SetFunc(SelectBar_GetSelect(&sb,0),CS1_TEST);
  SelectItem_SetFunc(SelectBar_GetSelect(&sb,1),CS2_TEST);
  while(1){
    PT_SERVICE();
    if(MAIN_KEY.keysign){
      MAIN_KEY.keysign = 0;
      if(MAIN_KEY.keycode == 10){
        SelectBar_ShiftUp(&sb);
        //NumBar_Add(&RES);
      }
      else if(MAIN_KEY.keycode == 14){
        SelectBar_ShiftDown(&sb);
        //NumBar_Cut(&RES);
      }
      else if(MAIN_KEY.keycode == 13){
        SelectBar_Hide(&sb);
        //NumBar_Hide(&CAP);
        //NumBar_Show(&RES);
        //NumBar_ShiftLeft(&RES);
      }
      else if(MAIN_KEY.keycode == 15){
        SelectBar_Show(&sb);
        //NumBar_ShiftRight(&RES);
        //NumBar_Hide(&RES);
        //NumBar_Show(&CAP);
      }
    }
  }
}