/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "FPGA.h"
extern "C"{
  #include "main.h"
}
const u32 CH1_REG = 0x20;
const u32 CH2_REG = 0x40;
//ÐÞÕýº¯Êý
double func(double x,double freq){
  return x-(2E-17*freq*freq + 3E-09*freq + 0.0002);
}
double getFreq(){
  u32 t_b,t_t;
  t_b = *(EXTERN_RAM)(CH1_REG+0x600a0000);
  t_b |= *(EXTERN_RAM)(CH1_REG+0x600a0002)<<16;
  t_t = *(EXTERN_RAM)(CH1_REG+0x600a0004);
  t_t |= *(EXTERN_RAM)(CH1_REG+0x600a0006)<<16;
  *(EXTERN_RAM)(CH1_REG+0x600a000c) = BIT(1);
  *(EXTERN_RAM)(CH1_REG+0x600a000c) = BIT(3);
  return 0.99999*((double)t_t)/t_b*25000000;
}
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  {
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure PB1 pin as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  }
  u16 i;
  while(1){
    //*(EXTERN_RAM)(0x600a0000) = ++i;
    //LCD_ShowNumBig_L(0,10,1,i,WHITE);
    //PC4
    //PB0
    u32 t_b,t_t,t_b2,t_t2;
    if(PBin(0) == 0)
    {
      t_b = *(EXTERN_RAM)(CH2_REG+0x600a0000);
      t_b |= *(EXTERN_RAM)(CH2_REG+0x600a0002)<<16;
      t_t = *(EXTERN_RAM)(CH2_REG+0x600a0004);
      t_t |= *(EXTERN_RAM)(CH2_REG+0x600a0006)<<16;  
      t_b2 = *(EXTERN_RAM)(CH2_REG+0x600a0008);
      t_b2 |= *(EXTERN_RAM)(CH2_REG+0x600a000a)<<16;  
      t_t2  = *(EXTERN_RAM)(CH1_REG+0x600a0008);
      t_t2 |= *(EXTERN_RAM)(CH1_REG+0x600a000a)<<16;
      LCD_ShowNumBig_L(0,10,0,t_b2,WHITE);
      LCD_ShowNumBig_L(0,10,1,t_t2,WHITE);
      LCD_ShowNumBig_L(0,10,2,t_b,WHITE);
      LCD_ShowNumBig_L(0,10,3,t_t,WHITE);
      
      LCD_ShowDoubleBig(0,4,LCD_STRING_LEFT,func(((double)t_t)/(t_b+t_t),getFreq()),WHITE);
      LCD_ShowDoubleBig(0,5,LCD_STRING_LEFT,((double)t_b2)/(t_t2),WHITE);
    }
  }
}