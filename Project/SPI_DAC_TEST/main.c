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
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_InitTypeDef SPI_InitStructure;
  Spi2_GPIO_Config();
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  //����ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //��
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //CPOL=0 ʱ�����յ�
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //CPHA=0 ���ݲ����1��
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //���NSS
  //���棡 ʹ�ÿ⺯��ʱ��APB2ʱ��Ϊ72M ��SPI��Ҫ����18M ����������Ҫ4��Ƶ
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  //4��Ƶ 18Mhz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //��λ��ǰ
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  //CRC7 Ƭ�Ӹ�λ�󣬸�λ��ֵĬ��Ϊ0x07����˽�����Ϊ7��
  SPI_Init(SPI2, &SPI_InitStructure);
  PT_DEBUG(3,"SPI2 USE MASTER!\r\n");
  SPI_Cmd(SPI2, ENABLE);
  while(1){
    PBout(12) = 0;
    SPI2_WriteByte(255);
  }
}
