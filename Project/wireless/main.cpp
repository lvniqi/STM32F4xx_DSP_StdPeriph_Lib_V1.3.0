/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "SelectBar_L.h"
extern "C"{
  #include "main.h"
}

NumBar_CPP t_numbar(10,0,400,0);
void SET_FUNC(NumBar_CPP* p,int value){
  if((USART1_BUFFER.TxBuffer).len <(USART1_BUFFER.TxBuffer).len_max-1 ){
      Usart_Send(&USART1_BUFFER,value);
      Usart_Flush();
    }
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
  Usart1_Dma_Open(9600);
  t_numbar.show();
  t_numbar.setActive(true);
  t_numbar.setFunc(SET_FUNC);
  Set_Key_Func(SET_K10,10);
  Set_Key_Func(SET_K14,14);
  Set_Key_Func(SET_K13,13);
  Set_Key_Func(SET_K15,15);
  {
    PT *t = (PT *)mymalloc(sizeof(PT));
    PT_INIT(t,1,KEY_SERVICE);
    PT_ADD_THREAD(t);
  }
  while(1){
    Usart1_Buffer_Rx_Service();
    Usart1_Buffer_Tx_Service();
    PT_SERVICE();
    //LCD_ShowNumBig_L(0,10,2,TIM3_SEND_DATA,WHITE);
    //LCD_ShowNumBig_L(0,10,1,TIM4_GET_DATA,WHITE);
    //LCD_ShowNumBig_L(0,10,0,TIM4_GET_COUNT,WHITE);
  }
}