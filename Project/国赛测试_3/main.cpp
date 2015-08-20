/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "FPGA.h"
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
  while(1){
    PT_SERVICE();
    //PC4
    //PB0
    u32 t_b,t_t,t_b2,t_t2;
    
    u16 reg = *(EXTERN_RAM)(MODE_REG[0]+0x600a000c);
    LCD_ShowNumBig_L(0,10,7,reg&0xff,WHITE);
    LCD_ShowNumBig_L(0,10,8,(reg&0xff00)>>8,WHITE);
    if(PBin(0) == 0)
    {
      t_b = *(EXTERN_RAM)(MODE_REG[1]+0x600a0000);
      t_b |= *(EXTERN_RAM)(MODE_REG[1]+0x600a0002)<<16;
      t_t = *(EXTERN_RAM)(MODE_REG[1]+0x600a0004);
      t_t |= *(EXTERN_RAM)(MODE_REG[1]+0x600a0006)<<16;  
      t_b2 = *(EXTERN_RAM)(MODE_REG[1]+0x600a0008);
      t_b2 |= *(EXTERN_RAM)(MODE_REG[1]+0x600a000a)<<16;  
      t_t2  = *(EXTERN_RAM)(MODE_REG[0]+0x600a0008);
      t_t2 |= *(EXTERN_RAM)(MODE_REG[0]+0x600a000a)<<16;
      LCD_ShowNumBig_L(0,10,0,t_b2,WHITE);
      LCD_ShowNumBig_L(0,10,1,t_t2,WHITE);
      LCD_ShowNumBig_L(0,10,2,t_b,WHITE);
      LCD_ShowNumBig_L(0,10,3,t_t,WHITE);
      
      LCD_ShowDoubleBig(0,4,LCD_STRING_LEFT,((double)t_t)/(t_b+t_t),WHITE);
      LCD_ShowDoubleBig(0,5,LCD_STRING_LEFT,((double)t_b2)/(t_t2),WHITE);
    }
    if(FREQ_RESULT<40){
      LCD_ShowDoubleBig(0,6,LCD_STRING_LEFT,FREQ_RESULT/1000,WHITE);
    }else{
      LCD_ShowDoubleBig(0,6,LCD_STRING_LEFT,Freq_Get()/1000,WHITE);
    }
  }
}