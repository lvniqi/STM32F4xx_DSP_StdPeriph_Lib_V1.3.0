/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "FPGA.h"
#include "Freq.h"
extern "C"{
  #include "main.h"
}
PT PT_t;
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  Freq_Init_ALL();
  PT_INIT(&PT_t,100,FREQ_GET_SERVICE);
  PT_ADD_THREAD(&PT_t);
  FPGA_EXIT_INIT();
  u16 i;
  NumBar_CPP t_numBar(15,0,(float)100000,(float)0);
  t_numBar.setLtag(String_L(3,0,0,2,WHITE));
  t_numBar.show();
  t_numBar.setRtag(String_L(3,0,"Hz",WHITE));
  t_numBar.setFunc(Freq_Value_UnitFix);
  while(1){
    PT_SERVICE();
    //PC4
    //PB0
    if(FREQ_RESULT<40 ){
      t_numBar.setValue(FREQ_RESULT);
    }else{
      t_numBar.setValue(Freq_Get());
    }
  }
}