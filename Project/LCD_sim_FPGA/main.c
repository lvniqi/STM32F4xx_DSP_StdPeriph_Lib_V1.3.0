

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#define FPGA_Clk_In() GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)
#define FPGA_Clk_Next() GPIO_ToggleBits(GPIOE,GPIO_Pin_2)
#define FPGA_Data_Get() GPIO_ReadInputData(GPIOA)
/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

void GPIO_Init_Test(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(
                  RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);//使能PORTBA
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  
  //PORTD复用推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2
                                |GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5
                                |GPIO_Pin_6|GPIO_Pin_7;				
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
u8 a[4096];
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
  u16 j;
  GPIO_Init_Test();
  GPIO_WriteBit(GPIOE,GPIO_Pin_2,0);
  while (1)
  {
    i = 0;
    j=0;
    if(FPGA_Clk_In())
    {
      while(1)
      {
        u16 i = 0;
        u8 temp;
        temp =FPGA_Clk_In(); 
        a[j++] = FPGA_Data_Get()&0xff;
        FPGA_Clk_Next();
        while(temp == FPGA_Clk_In() && i<20){i++;}
        if(i == 20)
        {
          FPGA_Clk_Next();
          break;
        }
      }
    }
    if(j != 0)
    {
      Lcd_Curve1.finish_flag = 0;
      (Lcd_Curve1.Now_x) = 0;
      for(int i=1;i<500;i++)
      {
        LCD_Curve_Show(&Lcd_Curve1,a[i],1);
      }
      char b[30];
      sprintf(b,"NUM:%d LEN:%d   ",a[0],j);
      LCD_ShowString(0,0,16,b,RED);
    }
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
