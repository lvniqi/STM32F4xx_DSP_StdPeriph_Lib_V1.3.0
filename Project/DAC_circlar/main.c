/* Includes ------------------------------------------------------------------*/
#include "main.h"
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  LCD_Init();
  ADS1220 t;
  ADS1220_Init(&t);
  TIM2_Configuration(2*651*64);
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO,true,DMA_Mode_Circular);
  DAC_Cmd(DAC_Channel_1,ENABLE);
  DMA_Cmd(DAC1_DMA_STREAM, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  //Dac_Init(DAC_Channel_1,DAC_Trigger_Software);
  
  while(1){
    Usart1_Buffer_Tx_Service();
    Usart1_Buffer_Rx_Service();
  }
}
