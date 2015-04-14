

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#include "DDS.h"
#include "math.h"
/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
//RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
Lcd_Curve Lcd_Curve1;
int count = 0;
int count2 = 0;
int main(void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  LCD_Init();
  LCD_Curve_Init(&Lcd_Curve1,10,10,RED);
  EXTI_init();
  /* Add your application code here */
  /* Insert 50 ms delay */
  Delay(5);
  /* Infinite loop */
  u16 i;
  u8 j;
  for(i=0;i<256;i++){
      *(EXTERN_RAM)_FPGA_RAM_POS(i) = 128+128*sin(2*3.14*i/256);
  }
  FPGA_Set_Freq(1000000);
  LCD_ShowNumBig_L(180,36,0,FREQ,YELLOW);
  while (1)
  {
    Freq_Service(1);
    delay_ms(500);
    /*for(i=0;i<256;i++){
      LCD_ShowNumBig(0,100,i,RED);
      int temp = *(EXTERN_RAM)(_FPGA_RAM_POS(i));
      LCD_ShowNumBig(0,0,temp,RED);
      delay_ms(100);
    }*/
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
