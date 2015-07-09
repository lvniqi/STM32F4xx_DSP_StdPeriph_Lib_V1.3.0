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
  while(1){
    PT_SERVICE();
    if(MAIN_KEY.keysign){
      if(MAIN_KEY.keycode == 5){
          SelectBar_ShiftUp(&sub_menu);
      }
      else if(MAIN_KEY.keycode == 9){
          SelectBar_ShiftDown(&sub_menu);
      }
      else if(MAIN_KEY.keycode == 8){
        SelectBar_ShiftDown(&main_menu);
      }
      else if(MAIN_KEY.keycode == 4){
        SelectBar_ShiftUp(&main_menu);
      }
      else if(MAIN_KEY.keycode == 10){
        if(NumBar_IsActive(&RES_SAMPLE)&&NumBar_IsShow(&RES_SAMPLE)){
          NumBar_Add(&RES_SAMPLE);
        }
        else if(NumBar_IsActive(&RES_RANGE)&&NumBar_IsShow(&RES_RANGE)){
          NumBar_Add(&RES_RANGE);
        }
        else if(NumBar_IsActive(&CAP_SAMPLE)&&NumBar_IsShow(&CAP_SAMPLE)){
          NumBar_Add(&CAP_SAMPLE);
        }
        else if(NumBar_IsActive(&CAP_RANGE)&&NumBar_IsShow(&CAP_RANGE)){
          NumBar_Add(&CAP_RANGE);
        }
      }
      else if(MAIN_KEY.keycode == 14){
        if(NumBar_IsActive(&RES_SAMPLE)&&NumBar_IsShow(&RES_SAMPLE)){
          NumBar_Cut(&RES_SAMPLE);
        }
        else if(NumBar_IsActive(&RES_RANGE)&&NumBar_IsShow(&RES_RANGE)){
          NumBar_Cut(&RES_RANGE);
        }
        else if(NumBar_IsActive(&CAP_SAMPLE)&&NumBar_IsShow(&CAP_SAMPLE)){
          NumBar_Cut(&CAP_SAMPLE);
        }
        else if(NumBar_IsActive(&CAP_RANGE)&&NumBar_IsShow(&CAP_RANGE)){
          NumBar_Cut(&CAP_RANGE);
        }
      }
      else if(MAIN_KEY.keycode == 13){
        if(NumBar_IsActive(&RES_SAMPLE)&&NumBar_IsShow(&RES_SAMPLE)){
          NumBar_ShiftLeft(&RES_SAMPLE);
        }
        else if(NumBar_IsActive(&RES_RANGE)&&NumBar_IsShow(&RES_RANGE)){
          NumBar_ShiftLeft(&RES_RANGE);
        }
        else if(NumBar_IsActive(&CAP_SAMPLE)&&NumBar_IsShow(&CAP_SAMPLE)){
          NumBar_ShiftLeft(&CAP_SAMPLE);
        }
        else if(NumBar_IsActive(&CAP_RANGE)&&NumBar_IsShow(&CAP_RANGE)){
          NumBar_ShiftLeft(&CAP_RANGE);
        }
      }
      else if(MAIN_KEY.keycode == 15){
        if(NumBar_IsActive(&RES_SAMPLE)&&NumBar_IsShow(&RES_SAMPLE)){
          NumBar_ShiftRight(&RES_SAMPLE);
        }
        else if(NumBar_IsActive(&RES_RANGE)&&NumBar_IsShow(&RES_RANGE)){
          NumBar_ShiftRight(&RES_RANGE);
        }
        else if(NumBar_IsActive(&CAP_SAMPLE)&&NumBar_IsShow(&CAP_SAMPLE)){
          NumBar_ShiftRight(&CAP_SAMPLE);
        }
        else if(NumBar_IsActive(&CAP_RANGE)&&NumBar_IsShow(&CAP_RANGE)){
          NumBar_ShiftRight(&CAP_RANGE);
        }
      }
      MAIN_KEY.keysign = 0;
    }
  }
}