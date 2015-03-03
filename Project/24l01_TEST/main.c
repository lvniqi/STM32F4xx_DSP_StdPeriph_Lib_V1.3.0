/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#include "USART.h"
#include "AD.h"
#include "DA.h"
#include "Timer.h"
#include "Pingpang.h"
#include "stm32f4xx_dac.h"
#include "spi.h"
#include "nrf2401.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
u8 a[32] = {1,2,3,4,5};
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  LCD_Init();
  EXTI_init();
  InitNrf2401();
  Nrf2401_Open();
  delay_ms(100);
  if(Nrf2401_Check()){
    LCD_ShowStringBig(10,40,"CONNECT OK!",RED);
  }
  NRF24L01_TX_Mode();
  NRF24L01_RX_Mode();
  Print_Details();
  while (1){
    NRF24L01_RxPacket(a);
    Usart1_Buffer_Rx_Service();
    Usart1_Buffer_Tx_Service();
    delay_ms(50);
    if(a[0] != 1){
      LCD_ShowStringBig(10,10,"GET!   ",RED);
    }
    else{
      LCD_ShowStringBig(10,10,"ERROR!",RED);
    }
  }
}
