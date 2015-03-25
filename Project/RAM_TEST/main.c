

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#include "DDS.h"
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

/* Private functions ---------------------------------------------------------*/
void Lcd_Show_Line(u16 x,u16 y,u16 color)
{
  /*
  L2R
  u8 a0 = 1;
  u8 temp, t1, t;
  LCD_SetCursor(x, y);		//设置光标位置
  LCD_WriteRAM_Prepare();	//开始写入GRAM
  for (t1 = 0; t1 < 8; t1++)
  {
    LCD_WRITE_COLOR(color);
  }*/
  /*
  R2L
   LCD_WRITE_CMD(0x0036);												//显示行列设置
  LCD_WRITE_DATA(0x00a1);
  u8 a0 = 1;
  u8 temp, t1, t;
  LCD_SetCursor(x, y);		//设置光标位置
  LCD_WriteRAM_Prepare();	//开始写入GRAM
  for (t1 = 0; t1 < 8; t1++)
  {
    LCD_WRITE_COLOR(color);
  }
  LCD_WRITE_CMD(0x0036);												//显示行列设置
  LCD_WRITE_DATA(0x0029);
  */
  /*B2T
  LCD_WRITE_CMD(0x0036);												//显示行列设置
  LCD_WRITE_DATA(0x0001);
  u8 a0 = 1;
  u8 temp, t1, t;
  LCD_SetCursor(x, y);		//设置光标位置
  LCD_WriteRAM_Prepare();	//开始写入GRAM
  for (t1 = 0; t1 < 8; t1++)
  {
    LCD_WRITE_COLOR(color);
  }
  LCD_WRITE_CMD(0x0036);												//显示行列设置
  LCD_WRITE_DATA(0x0029);
  */
  /*T2B
  LCD_WRITE_CMD(0x0036);												//显示行列设置
  LCD_WRITE_DATA(0x0041);
  u8 a0 = 1;
  u8 temp, t1, t;
  LCD_SetCursor(x, y);		//设置光标位置
  LCD_WriteRAM_Prepare();	//开始写入GRAM
  for (t1 = 0; t1 < 8; t1++)
  {
    LCD_WRITE_COLOR(color);
  }
  LCD_WRITE_CMD(0x0036);												//显示行列设置
  LCD_WRITE_DATA(0x0029);
  */
}
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
  volatile unsigned short * addr = (void*)0x600a0000;
  //while(1)
  {
    for(i=0;i<256;i++){
      if(i<128){
        FPGA_RAM_WRITE(i,i);
      }
      else{
        *(volatile unsigned short *)(_FPGA_RAM_ADD+((i)<<1)) = (256-i);
      }
    }
  }
  while (1)
  {
    i++;
    u16 temp = *addr;
    addr += 2;
    LCD_ShowNumBig(0,100,temp,RED);
    delay_ms(100);
    
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
