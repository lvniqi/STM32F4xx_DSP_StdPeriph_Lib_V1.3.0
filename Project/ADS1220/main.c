/* Includes ------------------------------------------------------------------*/
#include "main.h"
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  LCD_Init();
  ADS1220 t;
  ADS1220_Init(&t);
  while(1){
    Usart1_Buffer_Tx_Service();
    Usart1_Buffer_Rx_Service();
  }
}
