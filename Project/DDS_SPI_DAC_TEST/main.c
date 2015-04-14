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
#include "ADC082S805.h"
#include "DDS.h"
#include "math.h"
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  LCD_Init();
  EXTI_init();
  InitAdc082Spi();
  SPI_Cmd(SPI2, ENABLE);
  //DDS·¢ÉúÆ÷
  for(int i=0;i<256;i++){
      *(EXTERN_RAM)_FPGA_RAM_POS(i) = 128+128*sin(2*3.14*i/256);
  }
  static int i=0,j=50;
  static u32 k=1000;
  LCD_ShowStringBig(100,280,"DDS",YELLOW);
  LCD_ShowChinese(100+16*3,280,32,1,YELLOW);
  LCD_ShowStringBig(100,180,"K10:F-  K11:F+",YELLOW);
  LCD_ShowStringBig(100,150,"K12:A-  K13:A+",YELLOW);
  LCD_ShowStringBig(100,120,"K14:V-  K15:V+",YELLOW);
  LCD_ShowStringBig(0,0,"OFFSET:",YELLOW);
  Set_Adc082(1,i);
  LCD_ShowNumBig_L(220,130,0,i*37,YELLOW);
  LCD_ShowStringBig(220,0,"mV",YELLOW);
  
  LCD_ShowStringBig(0,30,"Vpp:",YELLOW);
  Set_Adc082(0,j);
  LCD_ShowNumBig_L(220,130,30,j*118,YELLOW);
  LCD_ShowStringBig(220,30,"mV",YELLOW);
  
  LCD_ShowStringBig(0,60,"FREQ:",YELLOW);
  FPGA_Set_Freq(k);
  LCD_ShowNumBig_L(220,130,60,k,YELLOW);
  LCD_ShowStringBig(220,60,"Hz",YELLOW);
  //LCD_ShowNumBig_L(220,130,0,FREQ,YELLOW);
  //SPI²âÊÔ
  
  while(1){
    
    if(MAIN_KEY.keysign){
      MAIN_KEY.keysign = 0;
      if(15 == MAIN_KEY.keycode){
        i+=1;
        if(i>255){
          i=255;
        }
        Set_Adc082(1,i);
        LCD_ShowNumBig_L(220,130,0,i*37,YELLOW);
        delay_ms(1);
      }
      else if(14 == MAIN_KEY.keycode){
        i-=1;
        if(i<0){
          i=0;
        }
        Set_Adc082(1,i);
        LCD_ShowNumBig_L(220,130,0,i*37,YELLOW);
        delay_ms(1);
      }
      else if(13 == MAIN_KEY.keycode){
        j+=1;
        if(j>255){
          j=255;
        }
        Set_Adc082(0,j);
        LCD_ShowNumBig_L(220,130,30,j*118,YELLOW);
        delay_ms(1);
      }
      else if(12 == MAIN_KEY.keycode){
        j-=1;
        if(j<0){
          j=0;
        }
        Set_Adc082(0,j);
        LCD_ShowNumBig_L(220,130,30,j*118,YELLOW);
        delay_ms(1);
      }
      else if(11 == MAIN_KEY.keycode){
        k+=100;
        if(k>5000000){
          k=5000000;
        }
        FPGA_Set_Freq(k);
        LCD_ShowNumBig_L(220,130,60,k,YELLOW);
      }
      else if(10 == MAIN_KEY.keycode){
        k-=100;
        if(k<0){
          j=0;
        }
        FPGA_Set_Freq(k);
        LCD_ShowNumBig_L(220,130,60,k,YELLOW);
      }
    }
  }
}
