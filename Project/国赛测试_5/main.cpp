/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "FPGA.h"
#include "Freq.h"
#include "Span.h"
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
  Impluse_Init_ALL();
  Span_Init_ALL();
  PT_INIT(&PT_t,100,FREQ_GET_SERVICE);
  PT_ADD_THREAD(&PT_t);
  FPGA_EXIT_INIT();
  u16 i;
  //频率显示
  NumBar_CPP numBar_freq(11,8,(float)100000,(float)0);
  numBar_freq.setLtag(String_L(0,0,0,2,WHITE));
  numBar_freq.setRtag(String_L(0,0,"Hz",WHITE));
  numBar_freq.show();
  numBar_freq.setFunc(Freq_Value_UnitFix);
  
  //周期显示
  NumBar_CPP numBar_tim(11,7,(float)100000,(float)0);
  numBar_tim.setLtag(String_L(3,0,2,2,WHITE));
  numBar_tim.setRtag(String_L(3,0,"ms",WHITE));
  numBar_tim.show();
  numBar_tim.setFunc(Tim_Value_UnitFix);
  
  //占空比显示
  NumBar_CPP numBar_impluse(11,6,(float)9999,(float)0);
  numBar_impluse.setLtag(String_L(3,0,4,3,WHITE));
  numBar_impluse.setRtag(String_L(3,0,"%",WHITE));
  numBar_impluse.show();
  
  //时间间隔显示
  NumBar_CPP numBar_span(15,5,(float)100000,(float)0);
  numBar_span.setLtag(String_L(3,0,7,4,WHITE));
  numBar_span.setRtag(String_L(3,0,"ms",WHITE));
  numBar_span.show();
  numBar_span.setFunc(Span_Value_UnitFix);
  
  //numBar_tim.setFunc(Tim_Value_UnitFix);
  
  while(1){
    
    PT_SERVICE();
    //PC4
    //PB0
    double t_freq = Freq_Get();
    numBar_freq.setValue(t_freq);
    if(t_freq>0.5){
      numBar_tim.setValue(1/t_freq);
    }
    numBar_span.setValue(Span_Get());
    numBar_impluse.setValue(100*Impluse_Get());
    LCD_ShowDoubleBig(0,3,LCD_STRING_LEFT,IMPLUSE_CHINNAL_3_RECV(),WHITE);
    LCD_ShowNumBig(0,2,LCD_STRING_LEFT,FPGA_Read_Register(2,0),WHITE);
    LCD_ShowNumBig(0,1,LCD_STRING_LEFT,FPGA_Read_Register(2,1),WHITE);
    //*(EXTERN_RAM)(MODE_REG[0]+0x600a0000) = 1;
  }
}