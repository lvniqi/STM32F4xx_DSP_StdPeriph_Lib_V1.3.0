/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#include "USART.h"
#include "AD.h"
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
void DAC_Config(void)
{
  DAC_GPIO_Config();
  DAC_InitTypeDef DAC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1,&DAC_InitStructure);
  DAC_Cmd(DAC_Channel_1,ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_R,0);
  DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}
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
  DAC_Config();
  ADC1_Init(ADC_Channel_3);
  Usart1_Dma_Open(115200);
  int j = 0;
  int k = 32;
  while (1){
    Ad_Get_Service();
    PingPang_Service();
    DAC_SetChannel1Data(DAC_Align_12b_R,j);
    j += k;
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
    _pingpang_data *temp = PingPang_Out(&pingpang_ad);
    if(temp){
      for(int i=0;i<LCD_CURVE_WIDTH;i++){
        LCD_Curve_Show(&Lcd_Curve1,(temp->data[i])>>4,1);
      }
      if(Lcd_Curve1.finish_flag == 1){
        LCD_Curve_Go_0(&Lcd_Curve1);
      }
    }
    PingPang_Process(temp);
    Usart1_Buffer_Tx_Service();
    if(j >= 4095){
      k = -32;
    }
    else if(j == 0){
      k = 32;
    }
  }
}

