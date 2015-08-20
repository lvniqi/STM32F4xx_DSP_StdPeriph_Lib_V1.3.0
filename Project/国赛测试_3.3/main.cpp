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
      
      LCD_ShowNumBig_L(0,10,0,t_b,WHITE);
      LCD_ShowNumBig_L(0,10,1,t_t,WHITE);
      
    }
  }
}