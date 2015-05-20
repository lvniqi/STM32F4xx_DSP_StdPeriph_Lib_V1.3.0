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
u16 getAvr(u8* p,u16 len);
u8 getVpp(u8* p,int len);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
Lcd_Curve Lcd_Curve1;
void test_gipo_init(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
int OFFSET=0;
int VPP_2 = 64;
//»º³åÊý×é
u8 temp[2000];
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  LCD_Init();
  LCD_Curve_Init(&Lcd_Curve1,10,10,YELLOW);
  EXTI_init();
  InitAdc082Spi();
  SPI_Cmd(SPI2, ENABLE);
  static int i=0,j=50;
  static u32 k=1000;
  //SPI²âÊÔ
  VPP_2 = 16;
  Set_Adc082(2,VPP_2);
  OFFSET = 0;
  Set_Adc082(3,0.41*128+112.3);
  test_gipo_init();
  /*while(1){
    PAout(7) = 0;
    *(EXTERN_RAM)(0x600B0004) = 255;
    delay_ms(10);
    PAout(7) = 1;
    delay_ms(10);
  }*/
  while(1){
    if(PBin(0) == 1){
      if(Lcd_Curve1.finish_flag){
        LCD_Curve_Go_0(&Lcd_Curve1);
      }
      for(int i=0;i<2000;i++){
        temp[i] = (0xff&*(EXTERN_RAM)0x600B0002);
      }
      u8 avr = getAvr(temp,2000);
      u8 vpp = getVpp(temp,2000);
      LCD_ShowNumBig_L(80,0,280,avr,YELLOW);
      LCD_ShowNumBig_L(160,80,280,vpp,YELLOW);
      if(MAIN_KEY.keysign){
        MAIN_KEY.keysign = 0;
        if(MAIN_KEY.keycode == 0x0f){
          VPP_2 += 10;
          if(VPP_2 >254){
            VPP_2 = 254;
          }
          Set_Adc082(2,15.45*log(VPP_2)-20.64);
          LCD_ShowNumBig_L(240,160,280,VPP_2,YELLOW);
        }
        else if(MAIN_KEY.keycode == 0x0e){
          VPP_2 -= 10;
          if(VPP_2<0){
            VPP_2 = 0;
          }
          Set_Adc082(2,15.45*log(VPP_2)-20.64);
          LCD_ShowNumBig_L(240,160,280,VPP_2,YELLOW);
        }
        else if(MAIN_KEY.keycode == 0x0d){
          OFFSET += 5;
          if(OFFSET>120){
            OFFSET = 120;
          }
          Set_Adc082(3,0.41*(OFFSET+128)+114.3);
          LCD_ShowNumBig_L(320,240,280,OFFSET,YELLOW);
        }
        else if(MAIN_KEY.keycode == 0x0c){
          OFFSET -= 5;
          if(OFFSET<-120){
            OFFSET = -120;
          }
          Set_Adc082(3,0.41*(OFFSET+128)+114.3);
          LCD_ShowNumBig_L(320,240,280,OFFSET,YELLOW);
        }
      }
      if(Lcd_Curve1.finish_flag){
        LCD_Curve_Go_0(&Lcd_Curve1);
      }
      for(int i=0;PBin(0) == 1;i++){
        LCD_Curve_Show(&Lcd_Curve1,*(EXTERN_RAM)0x600B0002,1);
      }
    }
    //LCD_ShowNumBig_L(400,220,60,*(EXTERN_RAM)0x600B0002,YELLOW);
  }
}

u8 getVpp(u8* p,int len){
  u8 max=p[0],min=p[0];
  for(int i=0;i<len;i++){
    if(p[i] > max){
      max = p[i];
    }
    else if(p[i] < min){
      min = p[i];
    }
  }
  return max-min;
}

u16 getAvr(u8* p,u16 len){
  u32 rst = 0;
  for(int i=0;i<len;i++){
    rst += p[i];
  }
  return rst/len;
}