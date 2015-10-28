

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_fsmc.h"
#include "LCD.h"
#include "exti.h"
#include "USART.h"
#define FPGA_Clk_In() GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)
#define FPGA_Clk_Next() GPIO_ToggleBits(GPIOE,GPIO_Pin_2)
#define FPGA_Data_Get() GPIO_ReadInputData(GPIOA)
#define TIMEOUT 1
/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;

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
Lcd_Curve Lcd_Curve1;
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  Usart1_Dma_Open(115200);
  LCD_Init();
  LCD_Curve_Init(&Lcd_Curve1,10,10,RED);
  EXTI_init();
  /* Add your application code here */
  /* Insert 50 ms delay */
  Delay(5);
  /* Infinite loop */
  u16 i;
  u16 j;
  u16 k=0;
  u16 l=0;
  GPIO_Init_Test();
  GPIO_WriteBit(GPIOE,GPIO_Pin_2,0);
  /*while (1)
  {
    LCD_Curve_Show(&Lcd_Curve1,FPGA_Data_Get()&0xff,1);
    if(Lcd_Curve1.finish_flag){
      Lcd_Curve1.finish_flag = 0;
      (Lcd_Curve1.Now_x) = 0;
    }
  }*/
  AR9331_UN_ADC();
  AR9331_EN_ADC();
  AR9331_SET_POS(1,875);
  AR9331_SET_ZOOM(1,875);
  AR9331_SET_FREQ(1,8);
  while (1){
    Usart1_Buffer_Tx_Service();
    Usart1_Buffer_Rx_Service();
    u8 t[7];
    AR9331_Read(t);
    i = 0;
    j=0;
    if(FPGA_Clk_In()){
      k++;
      while(1){
        u16 i = 0;
        u8 temp;
        temp =FPGA_Clk_In(); 
        a[j++] = FPGA_Data_Get()&0xff;
        FPGA_Clk_Next();
        while(temp == FPGA_Clk_In() && i<TIMEOUT){i++;}
        if(i == TIMEOUT){
          FPGA_Clk_Next();
          break;
        }
        if(j>4000){
          break;
        }
      }
    }
    if(j != 0){
      if(j != 3001){
        l++;
      }
      Lcd_Curve1.finish_flag = 0;
      (Lcd_Curve1.Now_x) = 0;
      for(int i=1;i<500;i++){
        LCD_Curve_Show(&Lcd_Curve1,a[i],1);
      }
      char b[30];
      sprintf(b,"NUM:%d LEN:%d COUNT:%dERROR:%d    ",a[0],j,k,l);
      //sprintf(b,"SEND:%d,READ:%d  ",k&0xff,USART_ReceiveData(USART1));
      LCD_ShowString(0,0,16,b,RED);
      j = 0;
    }
    if(MAIN_KEY.keysign){
      static u16 pos = 875;
      static u16 zoom = 875; 
      static u16 freq_div = 4;
      if(MAIN_KEY.keycode == 1){
        pos += 5;
      } 
      if(MAIN_KEY.keycode == 2){
        zoom += 5;
      }
      if(MAIN_KEY.keycode == 3){
        freq_div +=2;
      }
      if(MAIN_KEY.keycode == 5){
        pos -= 5;
      }
      if(MAIN_KEY.keycode == 6){
        zoom -= 5;
      }
      if(MAIN_KEY.keycode == 7){
        freq_div -=2;
      }
      AR9331_SET_FREQ(1,freq_div);
      AR9331_SET_POS(1,pos);
      AR9331_SET_ZOOM(1,zoom);
      char b[30];
      sprintf(b,"POS:%d ZOOM:%d FREQ:%dKhz  " ,pos,zoom,200*1000/freq_div);
      LCD_ShowString(0,300,16,b,RED);
      MAIN_KEY.keysign = 0;
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
