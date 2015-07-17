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
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  Gpio_Init();
  TIM3_Configuration();
  TIM4_Configuration();
  while(1){
    TIM3_SEND_DATA++;
    LCD_ShowNumBig_L(0,5,1,TIM3_SEND_DATA,WHITE);
    delay_ms(100);
    while(captureEnable);
    LCD_ShowNumBig_L(0,5,0,data,WHITE);
  }
}