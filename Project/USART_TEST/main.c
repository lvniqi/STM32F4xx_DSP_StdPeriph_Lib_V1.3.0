/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#include "USART.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
u32 count = 0;
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  LCD_Init();
  EXTI_init();
  Usart1_Dma_Open(115200);
  while (1){
    delay_ms(10);
    Usart1_Buffer_Rx_Service();
    Usart1_Buffer_Tx_Service();
    while((USART1_BUFFER.RxBuffer).len){
        if((USART1_BUFFER.TxBuffer).len >(USART1_BUFFER.TxBuffer).len_max-1 ){
            Usart1_Buffer_Tx_Service();
            break;
        }
        count++;
        Usart_Send(&USART1_BUFFER,Sequeue_Get_Front(&(USART1_BUFFER.RxBuffer)));
        Sequeue_Out_Queue(&(USART1_BUFFER.RxBuffer));
    }
    LCD_ShowNumBig(10, 10, count, GREEN);
  }
}

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */


/**
 * @}
 */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF
        FILE****/
