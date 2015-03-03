/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#include "USART.h"
#include "stm32f4xx_dac.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
u32 count = 0;
void ADC_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
}
void ADC_Config(void)
{
  ADC_GPIO_Config();
  ADC_InitTypeDef ADC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //单通道 无需扫描
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //连续转换 关闭
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不用外部触发,软件触发转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_3Cycles);
  ADC_Cmd(ADC1, ENABLE);
  ADC_SoftwareStartConv(ADC1);	   //软件启动转换
}
void DAC_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
}
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
  ADC_Config();
  DAC_Config();
  Usart1_Dma_Open(115200);
  int j = 0;
  int k = 1;
  while (1){
    DAC_SetChannel1Data(DAC_Align_12b_R,j);
    j += k;
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
    printf("should:%d\tget:%d\n",j,ADC_GetConversionValue(ADC1));
    delay_ms(0);
    LCD_Curve_Show(&Lcd_Curve1,ADC_GetConversionValue(ADC1)>>4,1);
    if(Lcd_Curve1.finish_flag == 1){
      LCD_Curve_Go_0(&Lcd_Curve1);
    }
    Usart1_Buffer_Tx_Service();
    ADC_SoftwareStartConv(ADC1);
    if(j == 4095){
      k = -1;
    }
    else if(j == 0){
      k = 1;
    }
  }
}

