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
  LCD_Curve_Init(&Lcd_Curve1,10,10,GREEN);
  EXTI_init();
  TIM2_Configuration();
  //¿ªÆôÆ¹ÅÒ»º³å
  PingPang_Data_Free_Init();
  //ADC_Soft_Config(ADC_Channel_6);
  Dac_Soft_Init(DAC_Channel_2);
  ADC1_Init(ADC_Channel_3);
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO);
  Usart1_Dma_Open(115200);
  int j = 0;
  int k = 1;
  while (1){
    Ad_Get_Service();
    PingPang_Service();
    Dac_Send_Service();
    _pingpang_data *temp = PingPang_Out(&pingpang_ad);
    if(temp){
      DAC_Soft_Set(DAC_Channel_2,j);
      j += k;
      LCD_Curve_Go_0(&Lcd_Curve1);
      for(int i=0;i<(temp->len);i++){
        LCD_Curve_Show(&Lcd_Curve1,(temp->data[i])>>4,1);
      }
      if(not pingpang_da.geted[0]){
        PingPang_In_By_PingPang(&pingpang_da,temp);
      }
      else {
        PingPang_Free(temp);
        temp = 0;
      }
    }
    
    Usart1_Buffer_Tx_Service();
    if(j >= 4095){
      k = -1;
    }
    else if(j == 0){
      k = 1;
    }
  }
}

