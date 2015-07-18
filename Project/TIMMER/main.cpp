/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "SelectBar_L.h"
extern "C"{
  #include "main.h"
}
void Gpio_Init(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  //B14 MISO∏¥”√
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  PEout(5) = 1;
}
struct{
  u16 d1;
  u16 d2;
}t_dat;

NumBar_CPP t_numbar(10,0,400,0);
void SET_FUNC(NumBar_CPP* p,int value){
  t_dat.d2 = value;
  t_dat.d1 = ~(t_dat.d2);
  TIM3_SEND_DATA = *(u32*)(&t_dat);
}
void SET_K10(void){
  t_numbar.add();
}
void SET_K14(void){
  t_numbar.cut();
}
void SET_K13(void){
  t_numbar.shiftLeft();
}
void SET_K15(void){
  t_numbar.shiftRight();
}
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  Gpio_Init();
  t_numbar.show();
  t_numbar.setActive(true);
  t_numbar.setFunc(SET_FUNC);
  Set_Key_Func(SET_K10,10);
  Set_Key_Func(SET_K14,14);
  Set_Key_Func(SET_K13,13);
  Set_Key_Func(SET_K15,15);
  TIM3_Configuration();
  TIM4_Configuration();{
    PT *t = (PT *)mymalloc(sizeof(PT));
    PT_INIT(t,1,KEY_SERVICE);
    PT_ADD_THREAD(t);
  }
  while(1){
    PT_SERVICE();
    //LCD_ShowNumBig_L(0,10,2,TIM3_SEND_DATA,WHITE);
    //LCD_ShowNumBig_L(0,10,1,TIM4_GET_DATA,WHITE);
    //LCD_ShowNumBig_L(0,10,0,TIM4_GET_COUNT,WHITE);
  }
}